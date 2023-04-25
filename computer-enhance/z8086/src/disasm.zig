const std = @import("std");
const testing = std.testing;
const log = std.log;

const decode = @import("decode.zig");

pub fn disasm(instr_it: anytype, raw_writer: anytype, min_comment_pad: u64) !void {
    try raw_writer.writeAll("bits 16\n");
    var counting_writer = std.io.countingWriter(raw_writer);
    const writer = counting_writer.writer();

    while (try instr_it.next()) |instr| {
        counting_writer.bytes_written = 0;
        if (instr.exclusive_prefix != .none) try writer.print("{s} ", .{@tagName(instr.exclusive_prefix)});
        try writer.writeAll(@tagName(instr.op));

        switch (instr.payload) {
            .mod => |mod| {
                try printModOperand(writer, mod.getDst(), instr.seg_override_prefix);
                try writer.writeByte(',');
                try printModOperand(writer, mod.getSrc(), instr.seg_override_prefix);
            },

            .mod_special => |mod_special| {
                if (mod_special.dst == .addr) {
                    switch (instr.op) {
                        .call, .callf, .jmp, .jmpf => {}, // Unambiguously a wide ptr.
                        else => try writer.print(" {s} PTR", .{if (mod_special.w) "WORD" else "BYTE"}),
                    }
                }
                try printModOperand(writer, mod_special.dst, instr.seg_override_prefix);
                switch (mod_special.src) {
                    .uimm => |uimm| try writer.print(", 0x{x}", .{uimm}),
                    .simm => |simm| try writer.print(", {s}0x{x}", .{ if (simm < 0) "-" else "", std.math.absCast(simm) }),
                    .cl => try writer.writeAll(", cl"),
                    .none => {},
                }
            },

            .addr => |addr| inline for (.{ addr.getDst(), addr.getSrc() }, 0..) |operand, i| {
                switch (operand) {
                    .reg => |reg| try writer.print(" {s}", .{@tagName(reg)}),
                    .addr => |a| try writer.print(
                        " {s}[0x{x}]",
                        .{ if (instr.seg_override_prefix != .none) @tagName(instr.seg_override_prefix) else "", a },
                    ),
                }
                if (i == 0) try writer.writeByte(',');
            },

            .data => |data| try writer.print(" {s}, 0x{x}", .{ @tagName(data.reg), data.imm }),
            .ip_off => |ip_off| try writer.print(" ${s}0x{x}", .{ if (ip_off >= 0) "+" else "-", std.math.absCast(ip_off) }),
            .interseg_addr => |interseg_addr| try writer.print(" 0x{x}:0x{x}", .{ interseg_addr.cs, interseg_addr.addr }),
            .regs => |regs| try writer.print(" {s}, {s}", .{ @tagName(regs.dst), @tagName(regs.src) }),
            .reg => |reg| try writer.print(" {s}", .{@tagName(reg)}),
            .imm => |imm| try writer.print(" 0x{x}", .{imm}),
            .none => {},
        }

        for (@min(min_comment_pad, counting_writer.bytes_written)..min_comment_pad) |_| try writer.writeByte(' ');
        try writer.writeAll("  ;");
        for (instr_it.getPrevBytes()) |b| try writer.print(" {x:0>2}", .{b});
        if (@import("builtin").mode == .Debug) try writer.print(" ({s})", .{@tagName(instr.payload)});
        try writer.writeByte('\n');
    }
}

fn printModOperand(
    writer: anytype,
    operand: decode.ModOperand,
    seg_override_prefix: decode.Instr.SegOverridePrefix,
) !void {
    switch (operand) {
        .reg => |reg| try writer.print(" {s}", .{@tagName(reg)}),
        .addr => |ea| {
            try writer.print(" {s}[{s}", .{
                if (seg_override_prefix != .none) @tagName(seg_override_prefix) else "",
                if (ea.regs != .none) @tagName(ea.regs) else "",
            });
            if (ea.off != 0) try writer.print("{s}0x{x}", .{ if (ea.regs != .none) "+" else "", ea.off });
            try writer.writeByte(']');
        },
    }
}

pub fn main() !u8 {
    var stdout_buffered = std.io.bufferedWriter(std.io.getStdOut().writer());
    defer flushOrLogErr(&stdout_buffered);
    const stdout = stdout_buffered.writer();

    var arena = std.heap.ArenaAllocator.init(std.heap.page_allocator);
    defer arena.deinit();
    var args_it = try std.process.argsWithAllocator(arena.allocator());
    defer args_it.deinit();
    _ = args_it.skip(); // Skip executable path.

    var file_i: usize = 0;
    while (args_it.next()) |file_path| : (file_i += 1) {
        const file = std.fs.cwd().openFile(file_path, .{}) catch |err| {
            log.err("Failed to open file \"{s}\": {!}", .{ file_path, err });
            return 1;
        };
        defer file.close();

        if (file_i > 0) try stdout.writeByte('\n');
        try stdout.print("; {s}\n", .{file_path});
        disasmOrLogErr(file.reader(), &stdout_buffered, file_path) catch return 1;
    }
    if (file_i == 0) {
        disasmOrLogErr(std.io.getStdIn().reader(), &stdout_buffered, "from standard input") catch return 1;
    }

    return 0;
}

fn disasmOrLogErr(reader: anytype, buffered_writer: anytype, name: []const u8) !void {
    var buffered_reader = std.io.bufferedReader(reader);
    var it = decode.instrIterator(buffered_reader.reader());
    disasm(&it, buffered_writer.writer(), 42) catch |err| {
        flushOrLogErr(buffered_writer);

        log.err("Failed to disassemble {s}: {!}", .{ name, err });
        if (it.getPrevBytes().len > 0) {
            var buf: [" 0xXX".len * @TypeOf(it).prev_bytes_capacity]u8 = undefined;
            var buf_stream = std.io.fixedBufferStream(&buf);
            for (it.getPrevBytes()) |b| buf_stream.writer().print(" 0x{x:0>2}", .{b}) catch unreachable;
            log.err("(while decoding bytes: {{{s} }})", .{buf_stream.getWritten()});
        }
        return err;
    };
}

inline fn flushOrLogErr(buffered_writer: anytype) void {
    buffered_writer.flush() catch |err| log.err("Failed to flush output: {!}", .{err});
}

// Following tests are based on the "Computer, Enhance!" perfaware listings:
// https://github.com/cmuratori/computer_enhance/blob/main/perfaware/part1
const testOpenListing = @import("test.zig").testOpenListing;

test "0042: completionist decode" {
    var file = try testOpenListing("0042_completionist_decode");
    defer file.close();
    var it = decode.instrIterator(file.reader());

    var output = std.ArrayList(u8).init(testing.allocator);
    defer output.deinit();
    try disasm(&it, output.writer(), 42);
    try testing.expectEqualStrings(
        \\bits 16
        \\mov si, bx                                  ; 89 de (mod)
        \\mov dh, al                                  ; 88 c6 (mod)
        \\mov cl, 0xc                                 ; b1 0c (data)
        \\mov ch, 0xf4                                ; b5 f4 (data)
        \\mov cx, 0xc                                 ; b9 0c 00 (data)
        \\mov cx, 0xfff4                              ; b9 f4 ff (data)
        \\mov dx, 0xf6c                               ; ba 6c 0f (data)
        \\mov dx, 0xf094                              ; ba 94 f0 (data)
        \\mov al, [bx+si]                             ; 8a 00 (mod)
        \\mov bx, [bp+di]                             ; 8b 1b (mod)
        \\mov dx, [bp]                                ; 8b 56 00 (mod)
        \\mov ah, [bx+si+0x4]                         ; 8a 60 04 (mod)
        \\mov al, [bx+si+0x1387]                      ; 8a 80 87 13 (mod)
        \\mov [bx+di], cx                             ; 89 09 (mod)
        \\mov [bp+si], cl                             ; 88 0a (mod)
        \\mov [bp], ch                                ; 88 6e 00 (mod)
        \\mov ax, [bx+di+0xdb]                        ; 8b 41 db (mod)
        \\mov [si+0xfed4], cx                         ; 89 8c d4 fe (mod)
        \\mov dx, [bx+0xe0]                           ; 8b 57 e0 (mod)
        \\mov BYTE PTR [bp+di], 0x7                   ; c6 03 07 (mod_special)
        \\mov WORD PTR [di+0x385], 0x15b              ; c7 85 85 03 5b 01 (mod_special)
        \\mov bp, [0x5]                               ; 8b 2e 05 00 (mod)
        \\mov bx, [0xd82]                             ; 8b 1e 82 0d (mod)
        \\mov ax, [0x9fb]                             ; a1 fb 09 (addr)
        \\mov ax, [0x10]                              ; a1 10 00 (addr)
        \\mov [0x9fa], ax                             ; a3 fa 09 (addr)
        \\mov [0xf], ax                               ; a3 0f 00 (addr)
        \\push WORD PTR [bp+si]                       ; ff 32 (mod_special)
        \\push WORD PTR [0xbb8]                       ; ff 36 b8 0b (mod_special)
        \\push WORD PTR [bx+di+0xe2]                  ; ff 71 e2 (mod_special)
        \\push cx                                     ; 51 (reg)
        \\push ax                                     ; 50 (reg)
        \\push dx                                     ; 52 (reg)
        \\push cs                                     ; 0e (reg)
        \\pop WORD PTR [bp+si]                        ; 8f 02 (mod_special)
        \\pop WORD PTR [0x3]                          ; 8f 06 03 00 (mod_special)
        \\pop WORD PTR [bx+di+0xf448]                 ; 8f 81 48 f4 (mod_special)
        \\pop sp                                      ; 5c (reg)
        \\pop di                                      ; 5f (reg)
        \\pop si                                      ; 5e (reg)
        \\pop ds                                      ; 1f (reg)
        \\xchg ax, [bp+0xfc18]                        ; 87 86 18 fc (mod)
        \\xchg bp, [bx+0x32]                          ; 87 6f 32 (mod)
        \\nop                                         ; 90 (none)
        \\xchg ax, dx                                 ; 92 (regs)
        \\xchg ax, sp                                 ; 94 (regs)
        \\xchg ax, si                                 ; 96 (regs)
        \\xchg ax, di                                 ; 97 (regs)
        \\xchg cx, dx                                 ; 87 ca (mod)
        \\xchg si, cx                                 ; 87 f1 (mod)
        \\xchg cl, ah                                 ; 86 cc (mod)
        \\in al, 0xc8                                 ; e4 c8 (data)
        \\in al, dx                                   ; ec (regs)
        \\in ax, dx                                   ; ed (regs)
        \\out ax, 0x2c                                ; e7 2c (data)
        \\out al, dx                                  ; ee (regs)
        \\xlat                                        ; d7 (none)
        \\lea ax, [bx+di+0x58c]                       ; 8d 81 8c 05 (mod)
        \\lea bx, [bp+0xce]                           ; 8d 5e ce (mod)
        \\lea sp, [bp+0xfc15]                         ; 8d a6 15 fc (mod)
        \\lea di, [bx+si+0xf9]                        ; 8d 78 f9 (mod)
        \\lds ax, [bx+di+0x58c]                       ; c5 81 8c 05 (mod)
        \\lds bx, [bp+0xce]                           ; c5 5e ce (mod)
        \\lds sp, [bp+0xfc15]                         ; c5 a6 15 fc (mod)
        \\lds di, [bx+si+0xf9]                        ; c5 78 f9 (mod)
        \\les ax, [bx+di+0x58c]                       ; c4 81 8c 05 (mod)
        \\les bx, [bp+0xce]                           ; c4 5e ce (mod)
        \\les sp, [bp+0xfc15]                         ; c4 a6 15 fc (mod)
        \\les di, [bx+si+0xf9]                        ; c4 78 f9 (mod)
        \\lahf                                        ; 9f (none)
        \\sahf                                        ; 9e (none)
        \\pushf                                       ; 9c (none)
        \\popf                                        ; 9d (none)
        \\add cx, [bp]                                ; 03 4e 00 (mod)
        \\add dx, [bx+si]                             ; 03 10 (mod)
        \\add [bp+di+0x1388], ah                      ; 00 a3 88 13 (mod)
        \\add [bx], al                                ; 00 07 (mod)
        \\add sp, 0x188                               ; 81 c4 88 01 (mod_special)
        \\add si, 0x5                                 ; 83 c6 05 (mod_special)
        \\add ax, 0x3e8                               ; 05 e8 03 (data)
        \\add ah, 0x1e                                ; 80 c4 1e (mod_special)
        \\add al, 0x9                                 ; 04 09 (data)
        \\add cx, bx                                  ; 01 d9 (mod)
        \\add ch, al                                  ; 00 c5 (mod)
        \\adc cx, [bp]                                ; 13 4e 00 (mod)
        \\adc dx, [bx+si]                             ; 13 10 (mod)
        \\adc [bp+di+0x1388], ah                      ; 10 a3 88 13 (mod)
        \\adc [bx], al                                ; 10 07 (mod)
        \\adc sp, 0x188                               ; 81 d4 88 01 (mod_special)
        \\adc si, 0x5                                 ; 83 d6 05 (mod_special)
        \\adc ax, 0x3e8                               ; 15 e8 03 (data)
        \\adc ah, 0x1e                                ; 80 d4 1e (mod_special)
        \\adc al, 0x9                                 ; 14 09 (data)
        \\adc cx, bx                                  ; 11 d9 (mod)
        \\adc ch, al                                  ; 10 c5 (mod)
        \\inc ax                                      ; 40 (reg)
        \\inc cx                                      ; 41 (reg)
        \\inc dh                                      ; fe c6 (mod_special)
        \\inc al                                      ; fe c0 (mod_special)
        \\inc ah                                      ; fe c4 (mod_special)
        \\inc sp                                      ; 44 (reg)
        \\inc di                                      ; 47 (reg)
        \\inc BYTE PTR [bp+0x3ea]                     ; fe 86 ea 03 (mod_special)
        \\inc WORD PTR [bx+0x27]                      ; ff 47 27 (mod_special)
        \\inc BYTE PTR [bx+si+0x5]                    ; fe 40 05 (mod_special)
        \\inc WORD PTR [bp+di+0xd8c4]                 ; ff 83 c4 d8 (mod_special)
        \\inc WORD PTR [0x2485]                       ; ff 06 85 24 (mod_special)
        \\inc BYTE PTR [bp]                           ; fe 46 00 (mod_special)
        \\aaa                                         ; 37 (none)
        \\daa                                         ; 27 (none)
        \\sub cx, [bp]                                ; 2b 4e 00 (mod)
        \\sub dx, [bx+si]                             ; 2b 10 (mod)
        \\sub [bp+di+0x1388], ah                      ; 28 a3 88 13 (mod)
        \\sub [bx], al                                ; 28 07 (mod)
        \\sub sp, 0x188                               ; 81 ec 88 01 (mod_special)
        \\sub si, 0x5                                 ; 83 ee 05 (mod_special)
        \\sub ax, 0x3e8                               ; 2d e8 03 (data)
        \\sub ah, 0x1e                                ; 80 ec 1e (mod_special)
        \\sub al, 0x9                                 ; 2c 09 (data)
        \\sub cx, bx                                  ; 29 d9 (mod)
        \\sub ch, al                                  ; 28 c5 (mod)
        \\sbb cx, [bp]                                ; 1b 4e 00 (mod)
        \\sbb dx, [bx+si]                             ; 1b 10 (mod)
        \\sbb [bp+di+0x1388], ah                      ; 18 a3 88 13 (mod)
        \\sbb [bx], al                                ; 18 07 (mod)
        \\sbb sp, 0x188                               ; 81 dc 88 01 (mod_special)
        \\sbb si, 0x5                                 ; 83 de 05 (mod_special)
        \\sbb ax, 0x3e8                               ; 1d e8 03 (data)
        \\sbb ah, 0x1e                                ; 80 dc 1e (mod_special)
        \\sbb al, 0x9                                 ; 1c 09 (data)
        \\sbb cx, bx                                  ; 19 d9 (mod)
        \\sbb ch, al                                  ; 18 c5 (mod)
        \\dec ax                                      ; 48 (reg)
        \\dec cx                                      ; 49 (reg)
        \\dec dh                                      ; fe ce (mod_special)
        \\dec al                                      ; fe c8 (mod_special)
        \\dec ah                                      ; fe cc (mod_special)
        \\dec sp                                      ; 4c (reg)
        \\dec di                                      ; 4f (reg)
        \\dec BYTE PTR [bp+0x3ea]                     ; fe 8e ea 03 (mod_special)
        \\dec WORD PTR [bx+0x27]                      ; ff 4f 27 (mod_special)
        \\dec BYTE PTR [bx+si+0x5]                    ; fe 48 05 (mod_special)
        \\dec WORD PTR [bp+di+0xd8c4]                 ; ff 8b c4 d8 (mod_special)
        \\dec WORD PTR [0x2485]                       ; ff 0e 85 24 (mod_special)
        \\dec BYTE PTR [bp]                           ; fe 4e 00 (mod_special)
        \\neg ax                                      ; f7 d8 (mod_special)
        \\neg cx                                      ; f7 d9 (mod_special)
        \\neg dh                                      ; f6 de (mod_special)
        \\neg al                                      ; f6 d8 (mod_special)
        \\neg ah                                      ; f6 dc (mod_special)
        \\neg sp                                      ; f7 dc (mod_special)
        \\neg di                                      ; f7 df (mod_special)
        \\neg BYTE PTR [bp+0x3ea]                     ; f6 9e ea 03 (mod_special)
        \\neg WORD PTR [bx+0x27]                      ; f7 5f 27 (mod_special)
        \\neg BYTE PTR [bx+si+0x5]                    ; f6 58 05 (mod_special)
        \\neg WORD PTR [bp+di+0xd8c4]                 ; f7 9b c4 d8 (mod_special)
        \\neg WORD PTR [0x2485]                       ; f7 1e 85 24 (mod_special)
        \\neg BYTE PTR [bp]                           ; f6 5e 00 (mod_special)
        \\cmp bx, cx                                  ; 39 cb (mod)
        \\cmp dh, [bp+0x186]                          ; 3a b6 86 01 (mod)
        \\cmp [bp+0x2], si                            ; 39 76 02 (mod)
        \\cmp bl, 0x14                                ; 80 fb 14 (mod_special)
        \\cmp BYTE PTR [bx], 0x22                     ; 80 3f 22 (mod_special)
        \\cmp ax, 0x5d65                              ; 3d 65 5d (data)
        \\aas                                         ; 3f (none)
        \\das                                         ; 2f (none)
        \\mul al                                      ; f6 e0 (mod_special)
        \\mul cx                                      ; f7 e1 (mod_special)
        \\mul WORD PTR [bp]                           ; f7 66 00 (mod_special)
        \\mul BYTE PTR [bx+di+0x1f4]                  ; f6 a1 f4 01 (mod_special)
        \\imul ch                                     ; f6 ed (mod_special)
        \\imul dx                                     ; f7 ea (mod_special)
        \\imul BYTE PTR [bx]                          ; f6 2f (mod_special)
        \\imul WORD PTR [0x250b]                      ; f7 2e 0b 25 (mod_special)
        \\aam                                         ; d4 0a (none)
        \\div bl                                      ; f6 f3 (mod_special)
        \\div sp                                      ; f7 f4 (mod_special)
        \\div BYTE PTR [bx+si+0xbae]                  ; f6 b0 ae 0b (mod_special)
        \\div WORD PTR [bp+di+0x3e8]                  ; f7 b3 e8 03 (mod_special)
        \\idiv ax                                     ; f7 f8 (mod_special)
        \\idiv si                                     ; f7 fe (mod_special)
        \\idiv BYTE PTR [bp+si]                       ; f6 3a (mod_special)
        \\idiv WORD PTR [bx+0x1ed]                    ; f7 bf ed 01 (mod_special)
        \\aad                                         ; d5 0a (none)
        \\cbw                                         ; 98 (none)
        \\cwd                                         ; 99 (none)
        \\not ah                                      ; f6 d4 (mod_special)
        \\not bl                                      ; f6 d3 (mod_special)
        \\not sp                                      ; f7 d4 (mod_special)
        \\not si                                      ; f7 d6 (mod_special)
        \\not WORD PTR [bp]                           ; f7 56 00 (mod_special)
        \\not BYTE PTR [bp+0x26b1]                    ; f6 96 b1 26 (mod_special)
        \\sal ah, 0x1                                 ; d0 e4 (mod_special)
        \\shr ax, 0x1                                 ; d1 e8 (mod_special)
        \\sar bx, 0x1                                 ; d1 fb (mod_special)
        \\rol cx, 0x1                                 ; d1 c1 (mod_special)
        \\ror dh, 0x1                                 ; d0 ce (mod_special)
        \\rcl sp, 0x1                                 ; d1 d4 (mod_special)
        \\rcr bp, 0x1                                 ; d1 dd (mod_special)
        \\sal WORD PTR [bp+0x5], 0x1                  ; d1 66 05 (mod_special)
        \\shr BYTE PTR [bx+si+0xff39], 0x1            ; d0 a8 39 ff (mod_special)
        \\sar BYTE PTR [bx+di+0xfed4], 0x1            ; d0 b9 d4 fe (mod_special)
        \\rol WORD PTR [bp], 0x1                      ; d1 46 00 (mod_special)
        \\ror WORD PTR [0x134a], 0x1                  ; d1 0e 4a 13 (mod_special)
        \\rcl BYTE PTR [0x3], 0x1                     ; d0 16 03 00 (mod_special)
        \\rcr WORD PTR [bx], 0x1                      ; d1 1f (mod_special)
        \\sal ah, cl                                  ; d2 e4 (mod_special)
        \\shr ax, cl                                  ; d3 e8 (mod_special)
        \\sar bx, cl                                  ; d3 fb (mod_special)
        \\rol cx, cl                                  ; d3 c1 (mod_special)
        \\ror dh, cl                                  ; d2 ce (mod_special)
        \\rcl sp, cl                                  ; d3 d4 (mod_special)
        \\rcr bp, cl                                  ; d3 dd (mod_special)
        \\sal WORD PTR [bp+0x5], cl                   ; d3 66 05 (mod_special)
        \\shr WORD PTR [bx+si+0xff39], cl             ; d3 a8 39 ff (mod_special)
        \\sar BYTE PTR [bx+di+0xfed4], cl             ; d2 b9 d4 fe (mod_special)
        \\rol BYTE PTR [bp], cl                       ; d2 46 00 (mod_special)
        \\ror BYTE PTR [0x134a], cl                   ; d2 0e 4a 13 (mod_special)
        \\rcl BYTE PTR [0x3], cl                      ; d2 16 03 00 (mod_special)
        \\rcr WORD PTR [bx], cl                       ; d3 1f (mod_special)
        \\and al, ah                                  ; 20 e0 (mod)
        \\and ch, cl                                  ; 20 cd (mod)
        \\and bp, si                                  ; 21 f5 (mod)
        \\and di, sp                                  ; 21 e7 (mod)
        \\and al, 0x5d                                ; 24 5d (data)
        \\and ax, 0x4fa8                              ; 25 a8 4f (data)
        \\and [bp+si+0xa], ch                         ; 20 6a 0a (mod)
        \\and [bx+di+0x3e8], dx                       ; 21 91 e8 03 (mod)
        \\and bx, [bp]                                ; 23 5e 00 (mod)
        \\and cx, [0x1120]                            ; 23 0e 20 11 (mod)
        \\and BYTE PTR [bp+0xd9], 0xef                ; 80 66 d9 ef (mod_special)
        \\and WORD PTR [bx+si+0xef14], 0x2858         ; 81 a0 14 ef 58 28 (mod_special)
        \\test bx, cx                                 ; 85 cb (mod)
        \\test [bp+0x186], dh                         ; 84 b6 86 01 (mod)
        \\test [bp+0x2], si                           ; 85 76 02 (mod)
        \\test bl, 0x14                               ; f6 c3 14 (mod_special)
        \\test BYTE PTR [bx], 0x22                    ; f6 07 22 (mod_special)
        \\test ax, 0x5d65                             ; a9 65 5d (data)
        \\or al, ah                                   ; 08 e0 (mod)
        \\or ch, cl                                   ; 08 cd (mod)
        \\or bp, si                                   ; 09 f5 (mod)
        \\or di, sp                                   ; 09 e7 (mod)
        \\or al, 0x5d                                 ; 0c 5d (data)
        \\or ax, 0x4fa8                               ; 0d a8 4f (data)
        \\or [bp+si+0xa], ch                          ; 08 6a 0a (mod)
        \\or [bx+di+0x3e8], dx                        ; 09 91 e8 03 (mod)
        \\or bx, [bp]                                 ; 0b 5e 00 (mod)
        \\or cx, [0x1120]                             ; 0b 0e 20 11 (mod)
        \\or BYTE PTR [bp+0xd9], 0xef                 ; 80 4e d9 ef (mod_special)
        \\or WORD PTR [bx+si+0xef14], 0x2858          ; 81 88 14 ef 58 28 (mod_special)
        \\xor al, ah                                  ; 30 e0 (mod)
        \\xor ch, cl                                  ; 30 cd (mod)
        \\xor bp, si                                  ; 31 f5 (mod)
        \\xor di, sp                                  ; 31 e7 (mod)
        \\xor al, 0x5d                                ; 34 5d (data)
        \\xor ax, 0x4fa8                              ; 35 a8 4f (data)
        \\xor [bp+si+0xa], ch                         ; 30 6a 0a (mod)
        \\xor [bx+di+0x3e8], dx                       ; 31 91 e8 03 (mod)
        \\xor bx, [bp]                                ; 33 5e 00 (mod)
        \\xor cx, [0x1120]                            ; 33 0e 20 11 (mod)
        \\xor BYTE PTR [bp+0xd9], 0xef                ; 80 76 d9 ef (mod_special)
        \\xor WORD PTR [bx+si+0xef14], 0x2858         ; 81 b0 14 ef 58 28 (mod_special)
        \\rep movsb                                   ; f3 a4 (none)
        \\rep cmpsb                                   ; f3 a6 (none)
        \\rep scasb                                   ; f3 ae (none)
        \\rep lodsb                                   ; f3 ac (none)
        \\rep movsw                                   ; f3 a5 (none)
        \\rep cmpsw                                   ; f3 a7 (none)
        \\rep scasw                                   ; f3 af (none)
        \\rep lodsw                                   ; f3 ad (none)
        \\rep stosb                                   ; f3 aa (none)
        \\rep stosw                                   ; f3 ab (none)
        \\call [0x9921]                               ; ff 16 21 99 (mod_special)
        \\call [bp+0x9c]                              ; ff 56 9c (mod_special)
        \\call sp                                     ; ff d4 (mod_special)
        \\call ax                                     ; ff d0 (mod_special)
        \\jmp ax                                      ; ff e0 (mod_special)
        \\jmp di                                      ; ff e7 (mod_special)
        \\jmp [0xc]                                   ; ff 26 0c 00 (mod_special)
        \\jmp [0x112b]                                ; ff 26 2b 11 (mod_special)
        \\ret 0xfff9                                  ; c2 f9 ff (imm)
        \\ret 0x1f4                                   ; c2 f4 01 (imm)
        \\ret                                         ; c3 (none)
        \\je $-0x2                                    ; 74 fe (ip_off)
        \\jl $-0x4                                    ; 7c fc (ip_off)
        \\jle $-0x6                                   ; 7e fa (ip_off)
        \\jb $-0x8                                    ; 72 f8 (ip_off)
        \\jbe $-0xa                                   ; 76 f6 (ip_off)
        \\jp $-0xc                                    ; 7a f4 (ip_off)
        \\jo $-0xe                                    ; 70 f2 (ip_off)
        \\js $-0x10                                   ; 78 f0 (ip_off)
        \\jne $-0x12                                  ; 75 ee (ip_off)
        \\jnl $-0x14                                  ; 7d ec (ip_off)
        \\jnle $-0x16                                 ; 7f ea (ip_off)
        \\jnb $-0x18                                  ; 73 e8 (ip_off)
        \\jnbe $-0x1a                                 ; 77 e6 (ip_off)
        \\jnp $-0x1c                                  ; 7b e4 (ip_off)
        \\jno $-0x1e                                  ; 71 e2 (ip_off)
        \\jns $-0x20                                  ; 79 e0 (ip_off)
        \\loop $-0x22                                 ; e2 de (ip_off)
        \\loope $-0x24                                ; e1 dc (ip_off)
        \\loopne $-0x26                               ; e0 da (ip_off)
        \\jcxz $-0x28                                 ; e3 d8 (ip_off)
        \\int 0xd                                     ; cd 0d (imm)
        \\int 0x3                                     ; cc (imm)
        \\into                                        ; ce (none)
        \\iret                                        ; cf (none)
        \\clc                                         ; f8 (none)
        \\cmc                                         ; f5 (none)
        \\stc                                         ; f9 (none)
        \\cld                                         ; fc (none)
        \\std                                         ; fd (none)
        \\cli                                         ; fa (none)
        \\sti                                         ; fb (none)
        \\hlt                                         ; f4 (none)
        \\wait                                        ; 9b (none)
        \\lock not BYTE PTR [bp+0x26b1]               ; f0 f6 96 b1 26 (mod_special)
        \\lock xchg al, [0x64]                        ; f0 86 06 64 00 (mod)
        \\mov al, cs:[bx+si]                          ; 2e 8a 00 (mod)
        \\mov bx, ds:[bp+di]                          ; 3e 8b 1b (mod)
        \\mov dx, es:[bp]                             ; 26 8b 56 00 (mod)
        \\mov ah, ss:[bx+si+0x4]                      ; 36 8a 60 04 (mod)
        \\and ss:[bp+si+0xa], ch                      ; 36 20 6a 0a (mod)
        \\or ds:[bx+di+0x3e8], dx                     ; 3e 09 91 e8 03 (mod)
        \\xor bx, es:[bp]                             ; 26 33 5e 00 (mod)
        \\cmp cx, es:[0x1120]                         ; 26 3b 0e 20 11 (mod)
        \\test BYTE PTR cs:[bp+0xd9], 0xef            ; 2e f6 46 d9 ef (mod_special)
        \\sbb WORD PTR cs:[bx+si+0xef14], 0x2858      ; 2e 81 98 14 ef 58 28 (mod_special)
        \\lock not BYTE PTR cs:[bp+0x26b1]            ; f0 2e f6 96 b1 26 (mod_special)
        \\call 0x7b:0x1c8                             ; 9a c8 01 7b 00 (interseg_addr)
        \\jmp 0x315:0x22                              ; ea 22 00 15 03 (interseg_addr)
        \\mov [bx+si+0x3b], es                        ; 8c 40 3b (mod)
        \\jmp $+0x6d9                                 ; e9 d9 06 (ip_off)
        \\call $+0x2ab6                               ; e8 b6 2a (ip_off)
        \\retf 0x4494                                 ; ca 94 44 (imm)
        \\ret 0x4498                                  ; c2 98 44 (imm)
        \\retf                                        ; cb (none)
        \\ret                                         ; c3 (none)
        \\call [bp+si+0xc6]                           ; ff 52 c6 (mod_special)
        \\callf [bp+si+0xc6]                          ; ff 5a c6 (mod_special)
        \\jmp [di]                                    ; ff 25 (mod_special)
        \\jmpf [di]                                   ; ff 2d (mod_special)
        \\jmp 0x5566:0x7788                           ; ea 88 77 66 55 (interseg_addr)
        \\
    , output.items);
}
