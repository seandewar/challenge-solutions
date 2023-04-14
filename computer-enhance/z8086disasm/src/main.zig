const std = @import("std");

pub fn main() !u8 {
    var stdout_buffered = std.io.bufferedWriter(std.io.getStdOut().writer());
    defer stdout_buffered.flush() catch |err| std.debug.panic("Failed to flush stdout: {!}", .{err});
    const stdout = stdout_buffered.writer();

    var arena = std.heap.ArenaAllocator.init(std.heap.page_allocator);
    defer arena.deinit();
    var args_it = try std.process.argsWithAllocator(arena.allocator());
    defer args_it.deinit();
    _ = args_it.skip(); // Skip executable path.

    var file_i: usize = 0;
    while (args_it.next()) |file_path| : (file_i += 1) {
        const file = std.fs.cwd().openFile(file_path, .{ .mode = .read_only }) catch |err| {
            std.log.err("Failed to open file \"{s}\": {!}", .{ file_path, err });
            return 1;
        };
        defer file.close();

        if (file_i > 0) try stdout.writeByte('\n');
        try stdout.print("; File: {s}\n", .{file_path});
        var file_reader_buffered = std.io.bufferedReader(file.reader());
        disassemble(file_reader_buffered.reader(), stdout) catch |err| {
            std.log.err("Failed to disassemble file \"{s}\": {!}", .{ file_path, err });
            return 1;
        };
    }

    if (file_i == 0) {
        var stdin_reader_buffered = std.io.bufferedReader(std.io.getStdIn().reader());
        disassemble(stdin_reader_buffered.reader(), stdout) catch |err| {
            std.log.err("Failed to disassemble from standard input: {!}", .{err});
            return 1;
        };
    }
    return 0;
}

fn disassemble(reader: anytype, writer: anytype) !void {
    try writer.writeAll("bits 16\n");

    while (true) {
        const first = reader.readByte() catch |err| switch (err) {
            error.EndOfStream => break,
            else => return err,
        };

        var second: u8 = undefined; // May need to read-ahead to identify some instructions.
        const mnemonic = switch (first) {
            0x00...0x05 => "add",
            0x06, 0x0e, 0x16, 0x1e, 0x50...0x57 => "push",
            0x07, 0x17, 0x1f, 0x58...0x5f => "pop",
            0x08...0x0d => "or",
            0x10...0x15 => "adc",
            0x18...0x1d => "sbb",
            0x20...0x25 => "and",
            0x26 => "es:",
            0x27 => "daa",
            0x28...0x2d => "sub",
            0x2e => "cs:",
            0x2f => "das",
            0x30...0x35 => "xor",
            0x36 => "ss:",
            0x37 => "aaa",
            0x38...0x3d => "cmp",
            0x3e => "ds:",
            0x3f => "aas",
            0x40...0x47 => "inc",
            0x48...0x4f => "dec",

            0xd0...0xd3 => blk: {
                second = try reader.readByte();
                break :blk switch ((second >> 3) & 0b111) {
                    0 => "rol",
                    1 => "ror",
                    2 => "rcl",
                    3 => "rcr",
                    4 => "sal",
                    5 => "shr",
                    7 => "sar",
                    else => {
                        std.log.err("Unknown instruction: 0x{x:0>2} 0x{x:0>2}", .{ first, second });
                        return error.UnknownInstr;
                    },
                };
            },

            0xd4...0xd5 => blk: {
                second = try reader.readByte();
                if (second != 0b00001010) {
                    std.log.err("Unknown instruction: 0x{x:0>2} 0x{x:0>2}", .{ first, second });
                    return error.UnknownInstr;
                }
                break :blk if (first == 0xd4) "aam" else "aad";
            },

            0x70 => "jo",
            0x71 => "jno",
            0x72 => "jb",
            0x73 => "jnb",
            0x74 => "je",
            0x75 => "jne",
            0x76 => "jbe",
            0x77 => "jnbe",
            0x78 => "js",
            0x79 => "jns",
            0x7a => "jp",
            0x7b => "jnp",
            0x7c => "jl",
            0x7d => "jnl",
            0x7e => "jle",
            0x7f => "jnle",

            0x80...0x83 => blk: {
                second = try reader.readByte();
                break :blk ([_][]const u8{
                    "add",
                    "or",
                    "adc",
                    "sbb",
                    "and",
                    "sub",
                    "xor",
                    "cmp",
                })[(second >> 3) & 0b111];
            },

            0x84...0x85, 0xa8...0xa9 => "test",
            0x86...0x87, 0x91...0x97 => "xchg",
            0x88...0x8c, 0x8e, 0xa0...0xa3, 0xb0...0xbf, 0xc6...0xc7 => "mov",
            0x8d => "lea",

            0x8f => blk: {
                second = try reader.readByte();
                if ((second >> 3) & 0b111 != 0b000) {
                    std.log.err("Unknown instruction: 0x{x:0>2} 0x{x:0>2}", .{ first, second });
                    return error.UnknownInstr;
                }
                break :blk "pop";
            },

            0x90 => "nop",
            0x98 => "cbw",
            0x99 => "cwd",
            0x9a, 0xe8 => "call",
            0x9b => "wait",
            0x9c => "pushf",
            0x9d => "popf",
            0x9e => "sahf",
            0x9f => "lahf",
            0xa4...0xa5 => "movs",
            0xa6...0xa7 => "cmps",
            0xaa...0xab => "stos",
            0xac...0xad => "lods",
            0xae...0xaf => "scas",
            0xc2...0xc3 => "ret",
            0xca...0xcb => "retf",
            0xc4 => "les",
            0xc5 => "lds",
            0xcc...0xcd => "int",
            0xce => "into",
            0xcf => "iret",
            0xd7 => "xlat",
            0xe0 => "loopne",
            0xe1 => "loope",
            0xe2 => "loop",
            0xe3 => "jcxz",
            0xe4...0xe5, 0xec...0xed => "in",
            0xe6...0xe7, 0xee...0xef => "out",
            0xe9...0xeb => "jmp",
            0xf0 => "lock",
            0xf2 => "repne",
            0xf3 => "rep",
            0xf4 => "hlt",
            0xf5 => "cmc",

            0xf6...0xf7 => blk: {
                second = try reader.readByte();
                break :blk switch ((second >> 3) & 0b111) {
                    0 => "test",
                    2 => "not",
                    3 => "neg",
                    4 => "mul",
                    5 => "imul",
                    6 => "div",
                    7 => "idiv",
                    else => {
                        std.log.err("Unknown instruction: 0x{x:0>2} 0x{x:0>2}", .{ first, second });
                        return error.UnknownInstr;
                    },
                };
            },

            0xf8 => "clc",
            0xf9 => "stc",
            0xfa => "cli",
            0xfb => "sti",
            0xfc => "cld",
            0xfd => "std",

            0xfe => blk: {
                second = try reader.readByte();
                break :blk switch ((second >> 3) & 0b111) {
                    0 => "inc",
                    1 => "dec",
                    else => {
                        std.log.err("Unknown instruction: 0x{x:0>2} 0x{x:0>2}", .{ first, second });
                        return error.UnknownInstr;
                    },
                };
            },

            0xff => blk: {
                second = try reader.readByte();
                break :blk switch ((second >> 3) & 0b111) {
                    0 => "inc",
                    1 => "dec",
                    2, 3 => "call",
                    4, 5 => "jmp",
                    6 => "push",
                    else => {
                        std.log.err("Unknown instruction: 0x{x:0>2} 0x{x:0>2}", .{ first, second });
                        return error.UnknownInstr;
                    },
                };
            },

            else => {
                std.log.err("Unknown instruction: 0x{x:0>2}", .{first});
                return error.UnknownInstr;
            },
        };
        try writer.writeAll(mnemonic);

        switch (first) {
            0x00...0x03,
            0x08...0x0b,
            0x10...0x13,
            0x18...0x1b,
            0x20...0x23,
            0x28...0x2b,
            0x30...0x33,
            0x38...0x3b,
            0x84...0x8c,
            0x8e,
            => try handleModInstr(reader, writer, first, (first >> 1) & 1 == 1),

            0x04...0x05,
            0x0c...0x0d,
            0x14...0x15,
            0x1c...0x1d,
            0x24...0x25,
            0x2c...0x2d,
            0x34...0x35,
            0x3c...0x3d,
            0xa8...0xa9,
            => {
                const w = first & 1 == 1;
                try handleDataInstr(reader, writer, 0, w, w);
            },

            0x06...0x07 => try writer.writeAll(" es"),
            0x0e => try writer.writeAll(" cs"),
            0x16...0x17 => try writer.writeAll(" ss"),
            0x1e...0x1f => try writer.writeAll(" ds"),
            0x40...0x5f => try writer.print(" {s}", .{getGpRegisterName(@intCast(u3, first & 7), true)}),
            0x70...0x7f, 0xe0...0xe3, 0xeb => try handleIpIncrementInstr(reader, writer, false),

            0x80...0x83 => try handleModSpecialInstr(reader, writer, first, second, true),
            0x8f, 0xfe...0xff => try handleModSpecialInstr(reader, writer, first, second, false),
            0x8d, 0xc4...0xc5 => try handleModInstr(reader, writer, first, false),
            0x91...0x97 => try writer.print(" ax, {s}", .{getGpRegisterName(@intCast(u3, first & 7), true)}),
            0x9a, 0xea => {
                const off = try reader.readIntLittle(u16);
                const seg = try reader.readIntLittle(u16);
                try writer.print(" 0x{x}:0x{x}", .{ seg, off });
            },
            0xa0...0xa3 => try handleAddrInstr(reader, writer, first),
            0xa4, 0xa6 => try writer.writeAll(" dest-str8, src-str8"),
            0xa5, 0xa7 => try writer.writeAll(" dest-str16, src-str16"),
            0xaa, 0xae => try writer.writeAll(" dest-str8"),
            0xab, 0xaf => try writer.writeAll(" dest-str16"),
            0xac => try writer.writeAll(" src-str8"),
            0xad => try writer.writeAll(" src-str16"),
            0xb0...0xbf => {
                const w = (first >> 3) & 1 == 1;
                try handleDataInstr(reader, writer, @intCast(u3, first & 0b111), w, w);
            },
            0xc2, 0xca => try writer.print(" 0x{x}", .{try reader.readIntLittle(u16)}),
            0xc6...0xc7 => try handleModSpecialInstr(reader, writer, first, try reader.readByte(), true),
            0xcc => try writer.writeAll(" 3"),
            0xcd => try writer.print(" 0x{x}", .{try reader.readByte()}),
            0xd0...0xd3 => {
                try handleModSpecialInstr(reader, writer, first, second, false);
                try writer.print(", {s}", .{if (first < 0xd2) "1" else "cl"});
            },
            0xe4...0xe7 => try handleDataInstr(reader, writer, 0, first & 1 == 1, false),
            0xe8...0xe9 => try handleIpIncrementInstr(reader, writer, true),
            0xec...0xef => try writer.print(" {s}, dx", .{getGpRegisterName(0, first & 1 == 1)}),
            0xf6...0xf7 => try handleModSpecialInstr(reader, writer, first, second, (second >> 3) & 0b111 == 0),

            else => {},
        }

        try writer.writeByte(switch (first) {
            0x26, 0x2e, 0x36, 0x3e, 0xf0 => ' ', // Some instructions are prefixes.
            else => '\n',
        });
    }

    try writer.writeByte('\n');
}

fn handleModInstr(reader: anytype, writer: anytype, first: u8, d: bool) !void {
    const w = first & 1 == 1;
    const second = try reader.readByte();
    const mod = @intCast(u2, (second >> 6) & 0b11);
    const rm = @intCast(u3, second & 0b111);
    const reg_name = if (first == 0x8c or first == 0x8e)
        ([_][]const u8{ "es", "ds", "ss", "cs" })[(second >> 3) & 0b11]
    else
        getGpRegisterName(@intCast(u3, (second >> 3) & 0b111), w);

    try writer.writeByte(' ');
    if (d) try writer.print("{s}, ", .{reg_name});
    try writeModOperand(writer, reader, mod, rm, w);
    if (!d) try writer.print(", {s}", .{reg_name});
}

fn handleModSpecialInstr(reader: anytype, writer: anytype, first: u8, second: u8, read_imm: bool) !void {
    const w = first & 1 == 1;
    const mod = @intCast(u2, (second >> 6) & 0b11);
    const special = @intCast(u3, (second >> 3) & 0b111);
    const rm = @intCast(u3, second & 0b111);

    try writer.writeByte(' ');
    if (first == 0xff and (special == 0b011 or special == 0b101)) try writer.writeAll("FAR ");
    if (mod != 3) try writer.print("{s} PTR ", .{if (w) "WORD" else "BYTE"});
    try writeModOperand(writer, reader, mod, rm, w);

    if (read_imm) {
        const imm = if (first == 0x83)
            @bitCast(u16, @as(i16, try reader.readByteSigned()))
        else if (w)
            try reader.readIntLittle(u16)
        else
            try reader.readByte();

        try writer.print(", 0x{x}", .{imm});
    }
}

fn writeModOperand(writer: anytype, reader: anytype, mod: u2, rm: u3, w: bool) !void {
    switch (mod) {
        0 => try writeEffectiveAddr(writer, mod, rm, if (rm == 0b110) try reader.readIntLittle(u16) else 0),
        1 => try writeEffectiveAddr(writer, mod, rm, try reader.readByte()),
        2 => try writeEffectiveAddr(writer, mod, rm, try reader.readIntLittle(u16)),
        3 => try writer.writeAll(getGpRegisterName(rm, w)),
    }
}

fn handleAddrInstr(reader: anytype, writer: anytype, first: u8) !void {
    const d = (first >> 1) & 1 == 1;
    const w = first & 1 == 1;
    const addr = try reader.readIntLittle(u16);

    try writer.writeByte(' ');
    if (!d) try writer.print("{s}, ", .{getGpRegisterName(0, w)});
    try writer.print("[0x{x}]", .{addr});
    if (d) try writer.print(", {s}", .{getGpRegisterName(0, w)});
}

fn handleDataInstr(reader: anytype, writer: anytype, reg: u3, reg_w: bool, data_w: bool) !void {
    const reg_name = getGpRegisterName(reg, reg_w);
    const imm = if (data_w) try reader.readIntLittle(u16) else try reader.readByte();
    try writer.print(" {s}, 0x{x}", .{ reg_name, imm });
}

fn handleIpIncrementInstr(reader: anytype, writer: anytype, w: bool) !void {
    const off = if (w) try reader.readIntLittle(i16) else try reader.readByteSigned();
    try writer.print(" ${s}0x{x}", .{ if (off >= 0) "+" else "-", std.math.absCast(off) });
}

fn getGpRegisterName(reg: u3, w: bool) []const u8 {
    return ([_][8][]const u8{
        .{ "al", "cl", "dl", "bl", "ah", "ch", "dh", "bh" },
        .{ "ax", "cx", "dx", "bx", "sp", "bp", "si", "di" },
    })[@boolToInt(w)][reg];
}

fn writeEffectiveAddr(writer: anytype, mod: u2, rm: u3, disp: u16) !void {
    const reg_math_strs = [_][]const u8{
        "bx+si",
        "bx+di",
        "bp+si",
        "bp+di",
        "si",
        "di",
        "bp",
        "bx",
    };

    switch (mod) {
        0 => if (rm == 0b110) {
            try writer.print("[0x{x}]", .{disp});
        } else {
            try writer.print("[{s}]", .{reg_math_strs[rm]});
        },
        1, 2 => try writer.print("[{s}+0x{x}]", .{ reg_math_strs[rm], disp }),
        3 => unreachable,
    }
}
