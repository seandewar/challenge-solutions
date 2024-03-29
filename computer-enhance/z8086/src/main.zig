const std = @import("std");
const log = std.log;
const testing = std.testing;

const decode = @import("decode.zig");
const disasm = @import("disasm.zig");
const Sim = @import("Sim.zig");

var stdout_buffered: std.io.BufferedWriter(4096, std.fs.File.Writer) = std.io.bufferedWriter(std.io.getStdOut().writer());
const stdout = stdout_buffered.writer();

inline fn flushStdOutOrLogErr() void {
    stdout_buffered.flush() catch |err| log.err("Failed to flush output: {!}", .{err});
}

pub fn panic(msg: []const u8, error_return_trace: ?*std.builtin.StackTrace, ret_addr: ?usize) noreturn {
    flushStdOutOrLogErr();
    std.builtin.default_panic(msg, error_return_trace, ret_addr);
}

const Options = struct {
    disasm: bool = true,
    sim: bool = true,
    sim_screen_path: ?[]const u8 = null,
    right_justify: u64 = 42,
};

pub fn main() !u8 {
    var arena = std.heap.ArenaAllocator.init(std.heap.page_allocator);
    defer arena.deinit();

    const allocator = arena.allocator();
    var args_it = try std.process.argsWithAllocator(allocator);
    var files = std.ArrayListUnmanaged([]const u8){};
    var options = Options{};
    defer flushStdOutOrLogErr();
    {
        const exe_path = args_it.next();
        var files_only = false;
        while (args_it.next()) |arg| {
            if (!files_only and arg.len >= 2 and arg[0] == '-') {
                const option = if (arg[1] == '-')
                    if (arg.len == 2) {
                        files_only = true;
                        continue;
                    } else if (std.mem.eql(u8, "--no-disasm", arg))
                        "-D"
                    else if (std.mem.eql(u8, "--no-sim", arg))
                        "-S"
                    else if (std.mem.eql(u8, "--help", arg))
                        "-h"
                    else if (std.mem.startsWith(u8, arg, "--justify=")) {
                        const val = arg["--justify=".len..];
                        options.right_justify = std.fmt.parseInt(std.meta.FieldType(Options, .right_justify), val, 10) catch {
                            log.err("Invalid \"--justify\" value: \"{s}\"", .{val});
                            return 1;
                        };
                        continue;
                    } else if (std.mem.eql(u8, arg, "--screen")) {
                        const path = args_it.next() orelse {
                            log.err("Missing \"--screen\" path", .{});
                            return 1;
                        };
                        options.sim_screen_path = path;
                        continue;
                    } else {
                        log.err("Unknown option: \"{s}\"", .{arg});
                        return 1;
                    }
                else
                    arg;

                for (option[1..]) |flag| switch (flag) {
                    'D' => options.disasm = false,
                    'S' => options.sim = false,
                    'h' => {
                        try stdout.print(
                            \\Usage: {s} [OPTION]... [FILE]...
                            \\Disassemble and simulate Intel 8086 binary FILE(s) to standard output.
                            \\
                            \\With no FILE, or when FILE is '-', standard input is read.
                            \\
                            \\  -D, --no-disasm   do not disassemble the binaries
                            \\  -S, --no-sim      do not simulate the binaries
                            \\  --screen <FILE>   interpret the contents of simulated memory at 0x0100 as
                            \\                    a 64x64 RGBA image and write it to <file> in BMP format
                            \\                    only one input FILE can be used if this option is set
                            \\  --justify=N       right-justify instruction info by at least N columns
                            \\  -h, --help        display this help and exit
                            \\
                        , .{exe_path orelse "z8086"});
                        return 0;
                    },
                    else => {
                        log.err("Unknown option: \"-{c}\"", .{flag});
                        return 1;
                    },
                };
                continue;
            }

            if (!std.mem.eql(u8, "-", arg) or files.items.len > 0) try files.append(allocator, arg);
        }
    }
    if (!options.disasm and !options.sim) {
        log.err("Cannot use \"-D\" and \"-S\" together", .{});
        return 1;
    } else if (!options.sim and options.sim_screen_path != null) {
        log.err("Cannot use \"--screen\" with \"-S\"", .{});
        return 1;
    } else if (options.sim_screen_path != null and files.items.len > 1) {
        log.err("Cannot specify more than one file with \"--screen\"", .{});
        return 1;
    }

    if (files.items.len > 0) {
        for (files.items, 0..) |file_path, i| {
            const file = std.fs.cwd().openFile(file_path, .{}) catch |err| {
                flushStdOutOrLogErr();
                log.err("Failed to open file \"{s}\": {!}", .{ file_path, err });
                return 1;
            };
            defer file.close();
            var file_buffered = std.io.bufferedReader(file.reader());

            if (i > 0) try stdout.writeByte('\n');
            try stdout.print("; {s}\n", .{file_path});
            doTheWork(file_buffered.reader(), file_path, options) catch return 1;
        }
    } else {
        var stdin_buffered = std.io.bufferedReader(std.io.getStdIn().reader());
        doTheWork(stdin_buffered.reader(), "from standard input", options) catch return 1;
    }

    return 0;
}

fn doTheWork(raw_reader: anytype, name: []const u8, options: Options) !void {
    std.debug.assert(options.disasm or options.sim);
    std.debug.assert(options.sim or options.sim_screen_path == null);

    // A custom stream that is used to read from `reader` while also populating
    // a `Sim.Memory` buffer to be used for simulating the read binary.
    //
    // This allows us to both disasm and simulate the binary without requiring
    // the whole file to be buffered if only disasm is wanted, and allows the
    // whole file to be disasm'd if it cannot be simulated because it's larger
    // than `Sim.Memory`. It also doesn't require the stream to be seekable for
    // rewinding if both disasm and simulation is wanted.
    const Stream = struct {
        sim_mem: ?std.BoundedArray(u8, @typeInfo(Sim.Memory).Array.len) = .{},
        inner_reader: @TypeOf(raw_reader),

        const Reader = std.io.Reader(*@This(), @TypeOf(raw_reader).Error, read);

        inline fn reader(self: *@This()) Reader {
            return .{ .context = self };
        }

        fn read(self: *@This(), buf: []u8) !usize {
            const slice = buf[0..try self.inner_reader.read(buf)];
            if (self.sim_mem) |*sim_mem| sim_mem.appendSlice(slice) catch {
                self.sim_mem = null;
            };
            return slice.len;
        }

        fn createSim(self: *@This()) !Sim {
            if (self.sim_mem) |*sim_mem| {
                // Make sure the whole stream is read.
                sim_mem.len += try self.inner_reader.readAll(sim_mem.unusedCapacitySlice());
                if (self.inner_reader.readByte()) |_| {
                    self.sim_mem = null;
                    return error.BinaryTooLarge;
                } else |err| if (err != error.EndOfStream) {
                    return err;
                }

                @memset(sim_mem.unusedCapacitySlice()[0..], 0);
                return .{ .mem = &sim_mem.buffer };
            } else {
                return error.BinaryTooLarge;
            }
        }
    };

    var stream = Stream{ .inner_reader = raw_reader };
    var had_errors = false;

    if (options.disasm) {
        var it = decode.instrIterator(stream.reader());
        doDisasm(&it, stdout, options.right_justify) catch |err| {
            flushStdOutOrLogErr();
            log.err("Failed to disassemble {s}: {!}", .{ name, err });
            if (it.getPrevBytes().len > 0) {
                var buf: [" 0xXX".len * @TypeOf(it).prev_bytes_capacity]u8 = undefined;
                var buf_stream = std.io.fixedBufferStream(&buf);
                for (it.getPrevBytes()) |b| buf_stream.writer().print(" 0x{x:0>2}", .{b}) catch unreachable;
                log.err("(while decoding bytes: {{{s} }})", .{buf_stream.getWritten()});
            }
            had_errors = true;
        };
    }

    if (options.sim) {
        var create_sim_result = stream.createSim();
        if (create_sim_result) |*sim| {
            doSim(sim, @intCast(u16, stream.sim_mem.?.len), stdout, options.right_justify) catch |err| {
                flushStdOutOrLogErr();
                log.err("Failed to simulate {s}: {!}", .{ name, err });
                log.err("(at address: $0x{x})", .{sim.ip});
                had_errors = true;
            };

            if (options.sim_screen_path) |path| write_screen: {
                const file = std.fs.cwd().createFile(path, .{}) catch |err| {
                    flushStdOutOrLogErr();
                    log.err("Failed to open {s} for writing the simulated screen: {!}", .{ path, err });
                    had_errors = true;
                    break :write_screen;
                };
                defer file.close();

                var buffered_writer = std.io.bufferedWriter(file.writer());
                writeSimScreenBmp(buffered_writer.writer(), sim.mem.*, 64, 64) catch |err| {
                    flushStdOutOrLogErr();
                    log.err("Failed to write simulated screen to {s}: {!}", .{ path, err });
                    had_errors = true;
                };
            }
        } else |err| {
            flushStdOutOrLogErr();
            log.err("Failed to simulate {s}: {!}", .{ name, err });
            had_errors = true;
        }
    }

    if (had_errors) return error.WorkHadErrors;
}

fn doDisasm(instr_it: anytype, raw_writer: anytype, right_justify: u64) !void {
    try raw_writer.writeAll("bits 16\n");
    var counting_writer = std.io.countingWriter(raw_writer);
    const writer = counting_writer.writer();

    while (try instr_it.next()) |instr| {
        counting_writer.bytes_written = 0;
        try disasm.writeInstr(instr, writer);

        const pad = @max(right_justify, counting_writer.bytes_written);
        for (counting_writer.bytes_written..pad) |_| try writer.writeByte(' ');
        try writer.writeAll(" ;");
        for (instr_it.getPrevBytes()) |b| try writer.print(" {x:0>2}", .{b});
        if (@import("builtin").mode == .Debug) try writer.print(" ({s})", .{@tagName(instr.payload)});
        try writer.writeByte('\n');
    }
}

fn doSim(sim: *Sim, end_ip: u16, raw_writer: anytype, right_justify: u64) !void {
    try raw_writer.writeAll("\n; Simulation:\n");
    var counting_writer = std.io.countingWriter(raw_writer);
    const writer = counting_writer.writer();

    while (sim.ip < end_ip) : (try writer.writeByte('\n')) {
        const old_flags = sim.flags;
        const instr_ip = sim.ip;
        const info = try sim.step();
        const instr = info.instr orelse break;

        counting_writer.bytes_written = 0;
        try writer.print("; $0x{x}: ", .{instr_ip});
        try disasm.writeInstr(instr, writer);
        const flags_changed = !old_flags.eql(sim.flags);
        if (info.change == .none and !flags_changed) continue; // Nothing else to print.

        const pad = @max(right_justify, counting_writer.bytes_written);
        for (counting_writer.bytes_written..pad) |_| try writer.writeByte(' ');
        try writer.writeAll(" (");

        switch (info.change) {
            .reg => |reg| try writer.print(
                "{s}: 0x{x} -> 0x{x}",
                .{ @tagName(reg.reg), reg.old_val, sim.readRegister(reg.reg) },
            ),
            .loop_old_ip => |old_ip| try writer.print(
                "ip: 0x{x} -> 0x{x}, cx: 0x{x} -> 0x{x}",
                .{ old_ip, sim.ip, sim.cx +% 1, sim.cx },
            ),
            inline .memb, .memw => |mem, tag| try writer.print(
                "{s} PTR [0x{x}]: 0x{x} -> 0x{x}",
                .{
                    if (tag == .memb) "BYTE" else "WORD",
                    mem.addr,
                    mem.old_val,
                    sim.readMemory(mem.addr, true),
                },
            ),
            .none => {},
        }
        if (flags_changed) {
            if (info.change != .none) try writer.writeAll(", ");
            try writer.writeAll("f: ");
            try printFlags(writer, old_flags);
            try writer.writeAll(" -> ");
            try printFlags(writer, sim.flags);
        }

        try writer.writeByte(')');
    }

    try writer.writeAll(";\n; Final (non-zero) registers:\n");
    inline for (@typeInfo(Sim).Struct.fields) |field| {
        if (field.type == u16 and @field(sim, field.name) != 0) {
            try writer.print("; {s}: 0x{x:0>4}\n", .{ field.name, @field(sim, field.name) });
        }
    }
    if (sim.flags.count() > 0) {
        try writer.writeAll("; flags: ");
        try printFlags(writer, sim.flags);
        try writer.writeByte('\n');
    }
}

fn writeSimScreenBmp(writer: anytype, mem: Sim.Memory, w: u16, h: u16) !void {
    const bitmap_size: u32 = 4 * w * h;

    // BMP Header (14 bytes).
    try writer.writeAll("BM"); // Magic.
    try writer.writeIntLittle(u32, 14 + 40 + bitmap_size); // File size.
    try writer.writeByteNTimes(0, 4); // Reserved.
    try writer.writeIntLittle(u32, 14 + 40); // Bitmap data start offset.

    // DIB Header (40 bytes; Windows BITMAPINFOHEADER format, as it's simple and supports 32bpp).
    try writer.writeIntLittle(u32, 40); // DIB header size.
    try writer.writeIntLittle(i32, w); // Bitmap width.
    try writer.writeIntLittle(i32, h); // Bitmap height.
    try writer.writeIntLittle(u16, 1); // Colour planes.
    try writer.writeIntLittle(u16, 32); // Bits per pixel (bpp).
    try writer.writeIntLittle(u32, 0); // Compression method (0 = no compression).
    try writer.writeIntLittle(u32, 0); // Bitmap size (we're not compressing, so can be 0).
    try writer.writeIntLittle(i32, 2835); // Horizontal pixels per metre.
    try writer.writeIntLittle(i32, 2835); // Vertical pixels per metre.
    try writer.writeIntLittle(u32, 0); // Number of colours (0 = 2^n).
    try writer.writeIntLittle(u32, 0); // Number of important colours (0 = all important).

    // Bitmap data.
    try writer.writeAll(mem[0x100..][0..bitmap_size]);
}

inline fn printFlags(writer: anytype, flags: Sim.Flags) !void {
    if (flags.count() == 0) return writer.writeByte('_');
    var flags_it = flags.iterator();
    while (flags_it.next()) |flag| try writer.writeByte(std.ascii.toUpper(@tagName(flag)[0]));
}

// Following tests are based on the "Computer, Enhance!" perfaware listings:
// https://github.com/cmuratori/computer_enhance/blob/main/perfaware/part1
const testOpenListing = @import("test.zig").testOpenListing;

test "0042: completionist decode (disasm)" {
    var file = try testOpenListing("0042_completionist_decode");
    defer file.close();
    var it = decode.instrIterator(file.reader());

    var output = std.ArrayList(u8).init(testing.allocator);
    defer output.deinit();
    try doDisasm(&it, output.writer(), 42);
    try testing.expectEqualStrings(
        \\bits 16
        \\mov si, bx                                 ; 89 de (mod)
        \\mov dh, al                                 ; 88 c6 (mod)
        \\mov cl, 0xc                                ; b1 0c (data)
        \\mov ch, 0xf4                               ; b5 f4 (data)
        \\mov cx, 0xc                                ; b9 0c 00 (data)
        \\mov cx, 0xfff4                             ; b9 f4 ff (data)
        \\mov dx, 0xf6c                              ; ba 6c 0f (data)
        \\mov dx, 0xf094                             ; ba 94 f0 (data)
        \\mov al, [bx+si]                            ; 8a 00 (mod)
        \\mov bx, [bp+di]                            ; 8b 1b (mod)
        \\mov dx, [bp]                               ; 8b 56 00 (mod)
        \\mov ah, [bx+si+0x4]                        ; 8a 60 04 (mod)
        \\mov al, [bx+si+0x1387]                     ; 8a 80 87 13 (mod)
        \\mov [bx+di], cx                            ; 89 09 (mod)
        \\mov [bp+si], cl                            ; 88 0a (mod)
        \\mov [bp], ch                               ; 88 6e 00 (mod)
        \\mov ax, [bx+di+0xdb]                       ; 8b 41 db (mod)
        \\mov [si+0xfed4], cx                        ; 89 8c d4 fe (mod)
        \\mov dx, [bx+0xe0]                          ; 8b 57 e0 (mod)
        \\mov BYTE PTR [bp+di], 0x7                  ; c6 03 07 (mod_special)
        \\mov WORD PTR [di+0x385], 0x15b             ; c7 85 85 03 5b 01 (mod_special)
        \\mov bp, [0x5]                              ; 8b 2e 05 00 (mod)
        \\mov bx, [0xd82]                            ; 8b 1e 82 0d (mod)
        \\mov ax, [0x9fb]                            ; a1 fb 09 (addr)
        \\mov ax, [0x10]                             ; a1 10 00 (addr)
        \\mov [0x9fa], ax                            ; a3 fa 09 (addr)
        \\mov [0xf], ax                              ; a3 0f 00 (addr)
        \\push WORD PTR [bp+si]                      ; ff 32 (mod_special)
        \\push WORD PTR [0xbb8]                      ; ff 36 b8 0b (mod_special)
        \\push WORD PTR [bx+di+0xe2]                 ; ff 71 e2 (mod_special)
        \\push cx                                    ; 51 (reg)
        \\push ax                                    ; 50 (reg)
        \\push dx                                    ; 52 (reg)
        \\push cs                                    ; 0e (reg)
        \\pop WORD PTR [bp+si]                       ; 8f 02 (mod_special)
        \\pop WORD PTR [0x3]                         ; 8f 06 03 00 (mod_special)
        \\pop WORD PTR [bx+di+0xf448]                ; 8f 81 48 f4 (mod_special)
        \\pop sp                                     ; 5c (reg)
        \\pop di                                     ; 5f (reg)
        \\pop si                                     ; 5e (reg)
        \\pop ds                                     ; 1f (reg)
        \\xchg ax, [bp+0xfc18]                       ; 87 86 18 fc (mod)
        \\xchg bp, [bx+0x32]                         ; 87 6f 32 (mod)
        \\nop                                        ; 90 (none)
        \\xchg ax, dx                                ; 92 (regs)
        \\xchg ax, sp                                ; 94 (regs)
        \\xchg ax, si                                ; 96 (regs)
        \\xchg ax, di                                ; 97 (regs)
        \\xchg cx, dx                                ; 87 ca (mod)
        \\xchg si, cx                                ; 87 f1 (mod)
        \\xchg cl, ah                                ; 86 cc (mod)
        \\in al, 0xc8                                ; e4 c8 (data)
        \\in al, dx                                  ; ec (regs)
        \\in ax, dx                                  ; ed (regs)
        \\out ax, 0x2c                               ; e7 2c (data)
        \\out al, dx                                 ; ee (regs)
        \\xlat                                       ; d7 (none)
        \\lea ax, [bx+di+0x58c]                      ; 8d 81 8c 05 (mod)
        \\lea bx, [bp+0xce]                          ; 8d 5e ce (mod)
        \\lea sp, [bp+0xfc15]                        ; 8d a6 15 fc (mod)
        \\lea di, [bx+si+0xf9]                       ; 8d 78 f9 (mod)
        \\lds ax, [bx+di+0x58c]                      ; c5 81 8c 05 (mod)
        \\lds bx, [bp+0xce]                          ; c5 5e ce (mod)
        \\lds sp, [bp+0xfc15]                        ; c5 a6 15 fc (mod)
        \\lds di, [bx+si+0xf9]                       ; c5 78 f9 (mod)
        \\les ax, [bx+di+0x58c]                      ; c4 81 8c 05 (mod)
        \\les bx, [bp+0xce]                          ; c4 5e ce (mod)
        \\les sp, [bp+0xfc15]                        ; c4 a6 15 fc (mod)
        \\les di, [bx+si+0xf9]                       ; c4 78 f9 (mod)
        \\lahf                                       ; 9f (none)
        \\sahf                                       ; 9e (none)
        \\pushf                                      ; 9c (none)
        \\popf                                       ; 9d (none)
        \\add cx, [bp]                               ; 03 4e 00 (mod)
        \\add dx, [bx+si]                            ; 03 10 (mod)
        \\add [bp+di+0x1388], ah                     ; 00 a3 88 13 (mod)
        \\add [bx], al                               ; 00 07 (mod)
        \\add sp, 0x188                              ; 81 c4 88 01 (mod_special)
        \\add si, 0x5                                ; 83 c6 05 (mod_special)
        \\add ax, 0x3e8                              ; 05 e8 03 (data)
        \\add ah, 0x1e                               ; 80 c4 1e (mod_special)
        \\add al, 0x9                                ; 04 09 (data)
        \\add cx, bx                                 ; 01 d9 (mod)
        \\add ch, al                                 ; 00 c5 (mod)
        \\adc cx, [bp]                               ; 13 4e 00 (mod)
        \\adc dx, [bx+si]                            ; 13 10 (mod)
        \\adc [bp+di+0x1388], ah                     ; 10 a3 88 13 (mod)
        \\adc [bx], al                               ; 10 07 (mod)
        \\adc sp, 0x188                              ; 81 d4 88 01 (mod_special)
        \\adc si, 0x5                                ; 83 d6 05 (mod_special)
        \\adc ax, 0x3e8                              ; 15 e8 03 (data)
        \\adc ah, 0x1e                               ; 80 d4 1e (mod_special)
        \\adc al, 0x9                                ; 14 09 (data)
        \\adc cx, bx                                 ; 11 d9 (mod)
        \\adc ch, al                                 ; 10 c5 (mod)
        \\inc ax                                     ; 40 (reg)
        \\inc cx                                     ; 41 (reg)
        \\inc dh                                     ; fe c6 (mod_special)
        \\inc al                                     ; fe c0 (mod_special)
        \\inc ah                                     ; fe c4 (mod_special)
        \\inc sp                                     ; 44 (reg)
        \\inc di                                     ; 47 (reg)
        \\inc BYTE PTR [bp+0x3ea]                    ; fe 86 ea 03 (mod_special)
        \\inc WORD PTR [bx+0x27]                     ; ff 47 27 (mod_special)
        \\inc BYTE PTR [bx+si+0x5]                   ; fe 40 05 (mod_special)
        \\inc WORD PTR [bp+di+0xd8c4]                ; ff 83 c4 d8 (mod_special)
        \\inc WORD PTR [0x2485]                      ; ff 06 85 24 (mod_special)
        \\inc BYTE PTR [bp]                          ; fe 46 00 (mod_special)
        \\aaa                                        ; 37 (none)
        \\daa                                        ; 27 (none)
        \\sub cx, [bp]                               ; 2b 4e 00 (mod)
        \\sub dx, [bx+si]                            ; 2b 10 (mod)
        \\sub [bp+di+0x1388], ah                     ; 28 a3 88 13 (mod)
        \\sub [bx], al                               ; 28 07 (mod)
        \\sub sp, 0x188                              ; 81 ec 88 01 (mod_special)
        \\sub si, 0x5                                ; 83 ee 05 (mod_special)
        \\sub ax, 0x3e8                              ; 2d e8 03 (data)
        \\sub ah, 0x1e                               ; 80 ec 1e (mod_special)
        \\sub al, 0x9                                ; 2c 09 (data)
        \\sub cx, bx                                 ; 29 d9 (mod)
        \\sub ch, al                                 ; 28 c5 (mod)
        \\sbb cx, [bp]                               ; 1b 4e 00 (mod)
        \\sbb dx, [bx+si]                            ; 1b 10 (mod)
        \\sbb [bp+di+0x1388], ah                     ; 18 a3 88 13 (mod)
        \\sbb [bx], al                               ; 18 07 (mod)
        \\sbb sp, 0x188                              ; 81 dc 88 01 (mod_special)
        \\sbb si, 0x5                                ; 83 de 05 (mod_special)
        \\sbb ax, 0x3e8                              ; 1d e8 03 (data)
        \\sbb ah, 0x1e                               ; 80 dc 1e (mod_special)
        \\sbb al, 0x9                                ; 1c 09 (data)
        \\sbb cx, bx                                 ; 19 d9 (mod)
        \\sbb ch, al                                 ; 18 c5 (mod)
        \\dec ax                                     ; 48 (reg)
        \\dec cx                                     ; 49 (reg)
        \\dec dh                                     ; fe ce (mod_special)
        \\dec al                                     ; fe c8 (mod_special)
        \\dec ah                                     ; fe cc (mod_special)
        \\dec sp                                     ; 4c (reg)
        \\dec di                                     ; 4f (reg)
        \\dec BYTE PTR [bp+0x3ea]                    ; fe 8e ea 03 (mod_special)
        \\dec WORD PTR [bx+0x27]                     ; ff 4f 27 (mod_special)
        \\dec BYTE PTR [bx+si+0x5]                   ; fe 48 05 (mod_special)
        \\dec WORD PTR [bp+di+0xd8c4]                ; ff 8b c4 d8 (mod_special)
        \\dec WORD PTR [0x2485]                      ; ff 0e 85 24 (mod_special)
        \\dec BYTE PTR [bp]                          ; fe 4e 00 (mod_special)
        \\neg ax                                     ; f7 d8 (mod_special)
        \\neg cx                                     ; f7 d9 (mod_special)
        \\neg dh                                     ; f6 de (mod_special)
        \\neg al                                     ; f6 d8 (mod_special)
        \\neg ah                                     ; f6 dc (mod_special)
        \\neg sp                                     ; f7 dc (mod_special)
        \\neg di                                     ; f7 df (mod_special)
        \\neg BYTE PTR [bp+0x3ea]                    ; f6 9e ea 03 (mod_special)
        \\neg WORD PTR [bx+0x27]                     ; f7 5f 27 (mod_special)
        \\neg BYTE PTR [bx+si+0x5]                   ; f6 58 05 (mod_special)
        \\neg WORD PTR [bp+di+0xd8c4]                ; f7 9b c4 d8 (mod_special)
        \\neg WORD PTR [0x2485]                      ; f7 1e 85 24 (mod_special)
        \\neg BYTE PTR [bp]                          ; f6 5e 00 (mod_special)
        \\cmp bx, cx                                 ; 39 cb (mod)
        \\cmp dh, [bp+0x186]                         ; 3a b6 86 01 (mod)
        \\cmp [bp+0x2], si                           ; 39 76 02 (mod)
        \\cmp bl, 0x14                               ; 80 fb 14 (mod_special)
        \\cmp BYTE PTR [bx], 0x22                    ; 80 3f 22 (mod_special)
        \\cmp ax, 0x5d65                             ; 3d 65 5d (data)
        \\aas                                        ; 3f (none)
        \\das                                        ; 2f (none)
        \\mul al                                     ; f6 e0 (mod_special)
        \\mul cx                                     ; f7 e1 (mod_special)
        \\mul WORD PTR [bp]                          ; f7 66 00 (mod_special)
        \\mul BYTE PTR [bx+di+0x1f4]                 ; f6 a1 f4 01 (mod_special)
        \\imul ch                                    ; f6 ed (mod_special)
        \\imul dx                                    ; f7 ea (mod_special)
        \\imul BYTE PTR [bx]                         ; f6 2f (mod_special)
        \\imul WORD PTR [0x250b]                     ; f7 2e 0b 25 (mod_special)
        \\aam                                        ; d4 0a (none)
        \\div bl                                     ; f6 f3 (mod_special)
        \\div sp                                     ; f7 f4 (mod_special)
        \\div BYTE PTR [bx+si+0xbae]                 ; f6 b0 ae 0b (mod_special)
        \\div WORD PTR [bp+di+0x3e8]                 ; f7 b3 e8 03 (mod_special)
        \\idiv ax                                    ; f7 f8 (mod_special)
        \\idiv si                                    ; f7 fe (mod_special)
        \\idiv BYTE PTR [bp+si]                      ; f6 3a (mod_special)
        \\idiv WORD PTR [bx+0x1ed]                   ; f7 bf ed 01 (mod_special)
        \\aad                                        ; d5 0a (none)
        \\cbw                                        ; 98 (none)
        \\cwd                                        ; 99 (none)
        \\not ah                                     ; f6 d4 (mod_special)
        \\not bl                                     ; f6 d3 (mod_special)
        \\not sp                                     ; f7 d4 (mod_special)
        \\not si                                     ; f7 d6 (mod_special)
        \\not WORD PTR [bp]                          ; f7 56 00 (mod_special)
        \\not BYTE PTR [bp+0x26b1]                   ; f6 96 b1 26 (mod_special)
        \\sal ah, 0x1                                ; d0 e4 (mod_special)
        \\shr ax, 0x1                                ; d1 e8 (mod_special)
        \\sar bx, 0x1                                ; d1 fb (mod_special)
        \\rol cx, 0x1                                ; d1 c1 (mod_special)
        \\ror dh, 0x1                                ; d0 ce (mod_special)
        \\rcl sp, 0x1                                ; d1 d4 (mod_special)
        \\rcr bp, 0x1                                ; d1 dd (mod_special)
        \\sal WORD PTR [bp+0x5], 0x1                 ; d1 66 05 (mod_special)
        \\shr BYTE PTR [bx+si+0xff39], 0x1           ; d0 a8 39 ff (mod_special)
        \\sar BYTE PTR [bx+di+0xfed4], 0x1           ; d0 b9 d4 fe (mod_special)
        \\rol WORD PTR [bp], 0x1                     ; d1 46 00 (mod_special)
        \\ror WORD PTR [0x134a], 0x1                 ; d1 0e 4a 13 (mod_special)
        \\rcl BYTE PTR [0x3], 0x1                    ; d0 16 03 00 (mod_special)
        \\rcr WORD PTR [bx], 0x1                     ; d1 1f (mod_special)
        \\sal ah, cl                                 ; d2 e4 (mod_special)
        \\shr ax, cl                                 ; d3 e8 (mod_special)
        \\sar bx, cl                                 ; d3 fb (mod_special)
        \\rol cx, cl                                 ; d3 c1 (mod_special)
        \\ror dh, cl                                 ; d2 ce (mod_special)
        \\rcl sp, cl                                 ; d3 d4 (mod_special)
        \\rcr bp, cl                                 ; d3 dd (mod_special)
        \\sal WORD PTR [bp+0x5], cl                  ; d3 66 05 (mod_special)
        \\shr WORD PTR [bx+si+0xff39], cl            ; d3 a8 39 ff (mod_special)
        \\sar BYTE PTR [bx+di+0xfed4], cl            ; d2 b9 d4 fe (mod_special)
        \\rol BYTE PTR [bp], cl                      ; d2 46 00 (mod_special)
        \\ror BYTE PTR [0x134a], cl                  ; d2 0e 4a 13 (mod_special)
        \\rcl BYTE PTR [0x3], cl                     ; d2 16 03 00 (mod_special)
        \\rcr WORD PTR [bx], cl                      ; d3 1f (mod_special)
        \\and al, ah                                 ; 20 e0 (mod)
        \\and ch, cl                                 ; 20 cd (mod)
        \\and bp, si                                 ; 21 f5 (mod)
        \\and di, sp                                 ; 21 e7 (mod)
        \\and al, 0x5d                               ; 24 5d (data)
        \\and ax, 0x4fa8                             ; 25 a8 4f (data)
        \\and [bp+si+0xa], ch                        ; 20 6a 0a (mod)
        \\and [bx+di+0x3e8], dx                      ; 21 91 e8 03 (mod)
        \\and bx, [bp]                               ; 23 5e 00 (mod)
        \\and cx, [0x1120]                           ; 23 0e 20 11 (mod)
        \\and BYTE PTR [bp+0xd9], 0xef               ; 80 66 d9 ef (mod_special)
        \\and WORD PTR [bx+si+0xef14], 0x2858        ; 81 a0 14 ef 58 28 (mod_special)
        \\test bx, cx                                ; 85 cb (mod)
        \\test [bp+0x186], dh                        ; 84 b6 86 01 (mod)
        \\test [bp+0x2], si                          ; 85 76 02 (mod)
        \\test bl, 0x14                              ; f6 c3 14 (mod_special)
        \\test BYTE PTR [bx], 0x22                   ; f6 07 22 (mod_special)
        \\test ax, 0x5d65                            ; a9 65 5d (data)
        \\or al, ah                                  ; 08 e0 (mod)
        \\or ch, cl                                  ; 08 cd (mod)
        \\or bp, si                                  ; 09 f5 (mod)
        \\or di, sp                                  ; 09 e7 (mod)
        \\or al, 0x5d                                ; 0c 5d (data)
        \\or ax, 0x4fa8                              ; 0d a8 4f (data)
        \\or [bp+si+0xa], ch                         ; 08 6a 0a (mod)
        \\or [bx+di+0x3e8], dx                       ; 09 91 e8 03 (mod)
        \\or bx, [bp]                                ; 0b 5e 00 (mod)
        \\or cx, [0x1120]                            ; 0b 0e 20 11 (mod)
        \\or BYTE PTR [bp+0xd9], 0xef                ; 80 4e d9 ef (mod_special)
        \\or WORD PTR [bx+si+0xef14], 0x2858         ; 81 88 14 ef 58 28 (mod_special)
        \\xor al, ah                                 ; 30 e0 (mod)
        \\xor ch, cl                                 ; 30 cd (mod)
        \\xor bp, si                                 ; 31 f5 (mod)
        \\xor di, sp                                 ; 31 e7 (mod)
        \\xor al, 0x5d                               ; 34 5d (data)
        \\xor ax, 0x4fa8                             ; 35 a8 4f (data)
        \\xor [bp+si+0xa], ch                        ; 30 6a 0a (mod)
        \\xor [bx+di+0x3e8], dx                      ; 31 91 e8 03 (mod)
        \\xor bx, [bp]                               ; 33 5e 00 (mod)
        \\xor cx, [0x1120]                           ; 33 0e 20 11 (mod)
        \\xor BYTE PTR [bp+0xd9], 0xef               ; 80 76 d9 ef (mod_special)
        \\xor WORD PTR [bx+si+0xef14], 0x2858        ; 81 b0 14 ef 58 28 (mod_special)
        \\rep movsb                                  ; f3 a4 (none)
        \\rep cmpsb                                  ; f3 a6 (none)
        \\rep scasb                                  ; f3 ae (none)
        \\rep lodsb                                  ; f3 ac (none)
        \\rep movsw                                  ; f3 a5 (none)
        \\rep cmpsw                                  ; f3 a7 (none)
        \\rep scasw                                  ; f3 af (none)
        \\rep lodsw                                  ; f3 ad (none)
        \\rep stosb                                  ; f3 aa (none)
        \\rep stosw                                  ; f3 ab (none)
        \\call [0x9921]                              ; ff 16 21 99 (mod_special)
        \\call [bp+0x9c]                             ; ff 56 9c (mod_special)
        \\call sp                                    ; ff d4 (mod_special)
        \\call ax                                    ; ff d0 (mod_special)
        \\jmp ax                                     ; ff e0 (mod_special)
        \\jmp di                                     ; ff e7 (mod_special)
        \\jmp [0xc]                                  ; ff 26 0c 00 (mod_special)
        \\jmp [0x112b]                               ; ff 26 2b 11 (mod_special)
        \\ret 0xfff9                                 ; c2 f9 ff (imm)
        \\ret 0x1f4                                  ; c2 f4 01 (imm)
        \\ret                                        ; c3 (none)
        \\je $+0x0                                   ; 74 fe (ip_off)
        \\jl $-0x2                                   ; 7c fc (ip_off)
        \\jle $-0x4                                  ; 7e fa (ip_off)
        \\jb $-0x6                                   ; 72 f8 (ip_off)
        \\jbe $-0x8                                  ; 76 f6 (ip_off)
        \\jp $-0xa                                   ; 7a f4 (ip_off)
        \\jo $-0xc                                   ; 70 f2 (ip_off)
        \\js $-0xe                                   ; 78 f0 (ip_off)
        \\jne $-0x10                                 ; 75 ee (ip_off)
        \\jnl $-0x12                                 ; 7d ec (ip_off)
        \\jnle $-0x14                                ; 7f ea (ip_off)
        \\jnb $-0x16                                 ; 73 e8 (ip_off)
        \\jnbe $-0x18                                ; 77 e6 (ip_off)
        \\jnp $-0x1a                                 ; 7b e4 (ip_off)
        \\jno $-0x1c                                 ; 71 e2 (ip_off)
        \\jns $-0x1e                                 ; 79 e0 (ip_off)
        \\loop $-0x20                                ; e2 de (ip_off)
        \\loope $-0x22                               ; e1 dc (ip_off)
        \\loopne $-0x24                              ; e0 da (ip_off)
        \\jcxz $-0x26                                ; e3 d8 (ip_off)
        \\int 0xd                                    ; cd 0d (imm)
        \\int 0x3                                    ; cc (imm)
        \\into                                       ; ce (none)
        \\iret                                       ; cf (none)
        \\clc                                        ; f8 (none)
        \\cmc                                        ; f5 (none)
        \\stc                                        ; f9 (none)
        \\cld                                        ; fc (none)
        \\std                                        ; fd (none)
        \\cli                                        ; fa (none)
        \\sti                                        ; fb (none)
        \\hlt                                        ; f4 (none)
        \\wait                                       ; 9b (none)
        \\lock not BYTE PTR [bp+0x26b1]              ; f0 f6 96 b1 26 (mod_special)
        \\lock xchg al, [0x64]                       ; f0 86 06 64 00 (mod)
        \\mov al, cs:[bx+si]                         ; 2e 8a 00 (mod)
        \\mov bx, ds:[bp+di]                         ; 3e 8b 1b (mod)
        \\mov dx, es:[bp]                            ; 26 8b 56 00 (mod)
        \\mov ah, ss:[bx+si+0x4]                     ; 36 8a 60 04 (mod)
        \\and ss:[bp+si+0xa], ch                     ; 36 20 6a 0a (mod)
        \\or ds:[bx+di+0x3e8], dx                    ; 3e 09 91 e8 03 (mod)
        \\xor bx, es:[bp]                            ; 26 33 5e 00 (mod)
        \\cmp cx, es:[0x1120]                        ; 26 3b 0e 20 11 (mod)
        \\test BYTE PTR cs:[bp+0xd9], 0xef           ; 2e f6 46 d9 ef (mod_special)
        \\sbb WORD PTR cs:[bx+si+0xef14], 0x2858     ; 2e 81 98 14 ef 58 28 (mod_special)
        \\lock not BYTE PTR cs:[bp+0x26b1]           ; f0 2e f6 96 b1 26 (mod_special)
        \\call 0x7b:0x1c8                            ; 9a c8 01 7b 00 (interseg_addr)
        \\jmp 0x315:0x22                             ; ea 22 00 15 03 (interseg_addr)
        \\mov [bx+si+0x3b], es                       ; 8c 40 3b (mod)
        \\jmp $+0x6dc                                ; e9 d9 06 (ip_off)
        \\call $+0x2ab9                              ; e8 b6 2a (ip_off)
        \\retf 0x4494                                ; ca 94 44 (imm)
        \\ret 0x4498                                 ; c2 98 44 (imm)
        \\retf                                       ; cb (none)
        \\ret                                        ; c3 (none)
        \\call [bp+si+0xc6]                          ; ff 52 c6 (mod_special)
        \\callf [bp+si+0xc6]                         ; ff 5a c6 (mod_special)
        \\jmp [di]                                   ; ff 25 (mod_special)
        \\jmpf [di]                                  ; ff 2d (mod_special)
        \\jmp 0x5566:0x7788                          ; ea 88 77 66 55 (interseg_addr)
        \\
    , output.items);
}

test "0052: memory add loop (simulation)" {
    var file = try testOpenListing("0052_memory_add_loop");
    defer file.close();

    var mem: Sim.Memory = undefined;
    const end_ip = try file.readAll(&mem);
    @memset(mem[end_ip..], 0);
    var sim = Sim{ .mem = &mem };

    var output = std.ArrayList(u8).init(testing.allocator);
    defer output.deinit();
    try doSim(&sim, @intCast(u16, end_ip), output.writer(), 42);
    try testing.expectEqualStrings(
        \\
        \\; Simulation:
        \\; $0x0: mov dx, 0x6                        (dx: 0x0 -> 0x6)
        \\; $0x3: mov bp, 0x3e8                      (bp: 0x0 -> 0x3e8)
        \\; $0x6: mov si, 0x0                        (si: 0x0 -> 0x0)
        \\; $0x9: mov [bp+si], si                    (WORD PTR [0x3e8]: 0x0 -> 0x0)
        \\; $0xb: add si, 0x2                        (si: 0x0 -> 0x2)
        \\; $0xe: cmp si, dx                         (f: _ -> CPAS)
        \\; $0x10: jne $-0x7                         (ip: 0x10 -> 0x9)
        \\; $0x9: mov [bp+si], si                    (WORD PTR [0x3ea]: 0x0 -> 0x2)
        \\; $0xb: add si, 0x2                        (si: 0x2 -> 0x4, f: CPAS -> _)
        \\; $0xe: cmp si, dx                         (f: _ -> CAS)
        \\; $0x10: jne $-0x7                         (ip: 0x10 -> 0x9)
        \\; $0x9: mov [bp+si], si                    (WORD PTR [0x3ec]: 0x0 -> 0x4)
        \\; $0xb: add si, 0x2                        (si: 0x4 -> 0x6, f: CAS -> P)
        \\; $0xe: cmp si, dx                         (f: P -> PZ)
        \\; $0x10: jne $-0x7
        \\; $0x12: mov bx, 0x0                       (bx: 0x0 -> 0x0)
        \\; $0x15: mov si, 0x0                       (si: 0x6 -> 0x0)
        \\; $0x18: mov cx, [bp+si]                   (cx: 0x0 -> 0x0)
        \\; $0x1a: add bx, cx                        (bx: 0x0 -> 0x0)
        \\; $0x1c: add si, 0x2                       (si: 0x0 -> 0x2, f: PZ -> _)
        \\; $0x1f: cmp si, dx                        (f: _ -> CPAS)
        \\; $0x21: jne $-0x9                         (ip: 0x21 -> 0x18)
        \\; $0x18: mov cx, [bp+si]                   (cx: 0x0 -> 0x2)
        \\; $0x1a: add bx, cx                        (bx: 0x0 -> 0x2, f: CPAS -> _)
        \\; $0x1c: add si, 0x2                       (si: 0x2 -> 0x4)
        \\; $0x1f: cmp si, dx                        (f: _ -> CAS)
        \\; $0x21: jne $-0x9                         (ip: 0x21 -> 0x18)
        \\; $0x18: mov cx, [bp+si]                   (cx: 0x2 -> 0x4)
        \\; $0x1a: add bx, cx                        (bx: 0x2 -> 0x6, f: CAS -> P)
        \\; $0x1c: add si, 0x2                       (si: 0x4 -> 0x6)
        \\; $0x1f: cmp si, dx                        (f: P -> PZ)
        \\; $0x21: jne $-0x9
        \\;
        \\; Final (non-zero) registers:
        \\; bx: 0x0006
        \\; cx: 0x0004
        \\; dx: 0x0006
        \\; bp: 0x03e8
        \\; si: 0x0006
        \\; ip: 0x0023
        \\; flags: PZ
        \\
    , output.items);
}

test "0055: challenge rectangle (simulation screen BMP)" {
    const expected = try std.fs.cwd().readFileAlloc(testing.allocator, "0055_expected.bmp", 32 * 1024);
    defer testing.allocator.free(expected);
    var actual = std.ArrayList(u8).init(testing.allocator);
    defer actual.deinit();

    var mem: Sim.Memory = undefined;
    _ = try Sim.testRunListing("0055_challenge_rectangle", &mem);
    try writeSimScreenBmp(actual.writer(), mem, 64, 64);
    try testing.expectEqualSlices(u8, expected, actual.items);
}
