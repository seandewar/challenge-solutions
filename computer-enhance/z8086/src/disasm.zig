const std = @import("std");
const log = std.log;

const decode = @import("decode.zig");

pub fn disasm(reader: anytype, writer: anytype) !void {
    try writer.writeAll("bits 16\n");

    var it = decode.instrIterator(reader);
    while (try it.next()) |instr| {
        try writer.writeAll(@tagName(instr.op));

        switch (instr.payload) {
            .mod => |mod| {
                try printModOperand(writer, mod.getDst());
                try writer.writeByte(',');
                try printModOperand(writer, mod.getSrc());
            },

            .mod_special => |mod_special| {
                try printModOperand(writer, mod_special.dst);
                switch (mod_special.src) {
                    .uimm => |uimm| try writer.print(", 0x{x}", .{uimm}),
                    .simm8 => |simm| try writer.print(", {s}0x{x}", .{ if (simm < 0) "-" else "", std.math.absCast(simm) }),
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

        try writer.writeAll("  ;");
        for (it.getPrevBytes()) |b| try writer.print(" {x:0>2}", .{b});
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
    defer flush(&stdout_buffered);
    const stdout = stdout_buffered.writer();

    var arena = std.heap.ArenaAllocator.init(std.heap.page_allocator);
    defer arena.deinit();
    var args_it = try std.process.argsWithAllocator(arena.allocator());
    defer args_it.deinit();
    _ = args_it.skip(); // Skip executable path.

    var file_i: usize = 0;
    while (args_it.next()) |file_path| : (file_i += 1) {
        const file = std.fs.cwd().openFile(file_path, .{ .mode = .read_only }) catch |err| {
            log.err("Failed to open file \"{s}\": {!}", .{ file_path, err });
            return 1;
        };
        defer file.close();
        var file_buffered = std.io.bufferedReader(file.reader());

        if (file_i > 0) try stdout.writeByte('\n');
        try stdout.print("; {s}\n", .{file_path});

        disasm(file_buffered.reader(), stdout_buffered.writer()) catch |err| {
            flush(&stdout_buffered);
            log.err("Failed to disassemble file \"{s}\": {!}", .{ file_path, err });
            return 1;
        };
    }

    if (file_i == 0) {
        var stdin_buffered = std.io.bufferedReader(std.io.getStdIn().reader());
        disasm(stdin_buffered.reader(), stdout_buffered.writer()) catch |err| {
            flush(&stdout_buffered);
            log.err("Failed to disassemble from standard input: {!}", .{err});
            return 1;
        };
    }

    return 0;
}

inline fn flush(buffered_writer: anytype) void {
    buffered_writer.flush() catch |err| log.err("Failed to flush output: {!}", .{err});
}
