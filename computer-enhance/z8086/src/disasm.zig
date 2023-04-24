const std = @import("std");
const log = std.log;

const decode = @import("decode.zig");

pub fn disasm(instr_it: anytype, raw_writer: anytype) !void {
    try raw_writer.writeAll("bits 16\n");
    var counting_writer = std.io.countingWriter(raw_writer);
    const writer = counting_writer.writer();

    while (try instr_it.next()) |instr| {
        counting_writer.bytes_written = 0;
        try writer.writeAll(@tagName(instr.op));

        switch (instr.payload) {
            .mod => |mod| {
                try printModOperand(writer, mod.getDst());
                try writer.writeByte(',');
                try printModOperand(writer, mod.getSrc());
            },

            .mod_special => |mod_special| {
                if (mod_special.dst == .addr) {
                    switch (instr.op) {
                        .call, .callf, .jmp, .jmpf => {}, // Unambiguously a wide ptr.
                        else => try writer.print(" {s} PTR", .{if (mod_special.w) "WORD" else "BYTE"}),
                    }
                }
                try printModOperand(writer, mod_special.dst);
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
                    .addr => |a| try writer.print(" [0x{x}]", .{a}),
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

        const min_comment_pad = 42;
        for (@min(min_comment_pad, counting_writer.bytes_written)..min_comment_pad) |_| try writer.writeByte(' ');
        try writer.writeAll("  ;");
        for (instr_it.getPrevBytes()) |b| try writer.print(" {x:0>2}", .{b});
        if (@import("builtin").mode == .Debug) try writer.print(" ({s})", .{@tagName(instr.payload)});
        try writer.writeByte('\n');
    }
}

fn printModOperand(writer: anytype, operand: decode.ModOperand) !void {
    switch (operand) {
        .reg => |reg| try writer.print(" {s}", .{@tagName(reg)}),
        .addr => |ea| {
            try writer.print(" [{s}", .{if (ea.regs != .none) @tagName(ea.regs) else ""});
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
    disasm(&it, buffered_writer.writer()) catch |err| {
        flushOrLogErr(buffered_writer);

        log.err("Failed to disassemble {s}: {!}", .{ name, err });
        if (it.getPrevBytes().len > 0) {
            var buf: [" 0xXX".len * 6]u8 = undefined;
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
