const std = @import("std");
const log = std.log;

pub fn main() !u8 {
    var stdout_buffered = std.io.bufferedWriter(std.io.getStdOut().writer());
    defer stdout_buffered.flush() catch |err| log.err("Failed to flush stdout: {!}", .{err});
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

        if (file_i > 0) try stdout.writeByte('\n');
        try stdout.print("; File: {s}\n", .{file_path});
        var file_reader_buffered = std.io.bufferedReader(file.reader());
        disassemble(file_reader_buffered.reader(), stdout) catch |err| {
            log.err("Failed to disassemble file \"{s}\": {!}", .{ file_path, err });
            return 1;
        };
    }

    if (file_i == 0) {
        var stdin_reader_buffered = std.io.bufferedReader(std.io.getStdIn().reader());
        disassemble(stdin_reader_buffered.reader(), stdout) catch |err| {
            log.err("Failed to disassemble from standard input: {!}", .{err});
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

        // May need to read-ahead to identify these instructions.
        var mod_spec_instr_info: ModSpecialInstrInfo = undefined;
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
                mod_spec_instr_info = try readModSpecialInstrInfo(reader);
                break :blk switch (mod_spec_instr_info.op) {
                    0 => "rol",
                    1 => "ror",
                    2 => "rcl",
                    3 => "rcr",
                    4 => "sal",
                    5 => "shr",
                    7 => "sar",
                    else => return unknownModSpecialInstr(first, mod_spec_instr_info),
                };
            },

            0xd4...0xd5 => blk: {
                mod_spec_instr_info = try readModSpecialInstrInfo(reader);
                if (mod_spec_instr_info.byte != 0b1010) {
                    return unknownModSpecialInstr(first, mod_spec_instr_info);
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
                mod_spec_instr_info = try readModSpecialInstrInfo(reader);
                break :blk switch (mod_spec_instr_info.op) {
                    0 => "add",
                    1 => "or",
                    2 => "adc",
                    3 => "sbb",
                    4 => "and",
                    5 => "sub",
                    6 => "xor",
                    7 => "cmp",
                };
            },

            0x84...0x85, 0xa8...0xa9 => "test",
            0x86...0x87, 0x91...0x97 => "xchg",
            0x88...0x8c, 0x8e, 0xa0...0xa3, 0xb0...0xbf, 0xc6...0xc7 => "mov",
            0x8d => "lea",

            0x8f => blk: {
                mod_spec_instr_info = try readModSpecialInstrInfo(reader);
                if (mod_spec_instr_info.op != 0) return unknownModSpecialInstr(first, mod_spec_instr_info);
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
                mod_spec_instr_info = try readModSpecialInstrInfo(reader);
                break :blk switch (mod_spec_instr_info.op) {
                    0 => "test",
                    2 => "not",
                    3 => "neg",
                    4 => "mul",
                    5 => "imul",
                    6 => "div",
                    7 => "idiv",
                    else => return unknownModSpecialInstr(first, mod_spec_instr_info),
                };
            },

            0xf8 => "clc",
            0xf9 => "stc",
            0xfa => "cli",
            0xfb => "sti",
            0xfc => "cld",
            0xfd => "std",

            0xfe => blk: {
                mod_spec_instr_info = try readModSpecialInstrInfo(reader);
                break :blk switch (mod_spec_instr_info.op) {
                    0 => "inc",
                    1 => "dec",
                    else => return unknownModSpecialInstr(first, mod_spec_instr_info),
                };
            },

            0xff => blk: {
                mod_spec_instr_info = try readModSpecialInstrInfo(reader);
                break :blk switch (mod_spec_instr_info.op) {
                    0 => "inc",
                    1 => "dec",
                    2, 3 => "call",
                    4, 5 => "jmp",
                    6 => "push",
                    else => return unknownModSpecialInstr(first, mod_spec_instr_info),
                };
            },

            else => {
                log.err("Unknown instruction: 0x{x:0>2}", .{first});
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
            0x84...0x8e,
            0xc4...0xc5,
            => try handleModInstr(reader, writer, first),

            0x04...0x05,
            0x0c...0x0d,
            0x14...0x15,
            0x1c...0x1d,
            0x24...0x25,
            0x2c...0x2d,
            0x34...0x35,
            0x3c...0x3d,
            0xa8...0xa9,
            0xb0...0xbf,
            0xe4...0xe7,
            => try handleDataInstr(reader, writer, first),

            0x06...0x07 => try writer.writeAll(" es"),
            0x0e => try writer.writeAll(" cs"),
            0x16...0x17 => try writer.writeAll(" ss"),
            0x1e...0x1f => try writer.writeAll(" ds"),
            0x40...0x5f => try writer.print(" {s}", .{getGpRegisterName(@intCast(u3, first & 7), true)}),

            0x70...0x7f,
            0xe0...0xe3,
            0xe8...0xe9,
            0xeb,
            => try handleIpIncrInstr(reader, writer, first),

            0x80...0x83,
            0x8f,
            0xc6...0xc7,
            0xd0...0xd3,
            0xf6...0xf7,
            0xfe...0xff,
            => try handleModSpecialInstr(reader, writer, first, mod_spec_instr_info),

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

            0xc2, 0xca => try writer.print(" 0x{x}", .{try reader.readIntLittle(u16)}),
            0xcc => try writer.writeAll(" 3"),
            0xcd => try writer.print(" 0x{x}", .{try reader.readByte()}),
            0xec...0xef => try writer.print(" {s}, dx", .{getGpRegisterName(0, first & 1 == 1)}),
            else => {},
        }

        try writer.writeByte(switch (first) {
            // TODO: handle the cs:-like prefixes properly; they should be just before the [0x...] operand.
            0x26, 0x2e, 0x36, 0x3e, 0xf0 => ' ', // Some instructions are prefixes.
            else => '\n',
        });
    }

    try writer.writeByte('\n');
}

const ModSpecialInstrInfo = struct { byte: u8, op: u3 };
fn readModSpecialInstrInfo(reader: anytype) !ModSpecialInstrInfo {
    const byte = try reader.readByte();
    return .{ .byte = byte, .op = @intCast(u3, (byte >> 3) & 0b111) };
}

fn unknownModSpecialInstr(first: u8, info: ModSpecialInstrInfo) error{UnknownInstr} {
    log.err("Unknown instruction: 0x{x:0>2} 0x{x:0>2}", .{ first, info.byte });
    return error.UnknownInstr;
}

fn handleModInstr(reader: anytype, writer: anytype, first: u8) !void {
    const w = first & 1 == 1;
    const d = switch (first) {
        0x8d, 0xc4...0xc5 => false,
        else => (first >> 1) & 1 == 1,
    };

    const second = try reader.readByte();
    const mod = @intCast(u2, (second >> 6) & 0b11);
    const rm = @intCast(u3, second & 0b111);
    const reg_name = switch (first) {
        0x8c, 0x8e => ([_][]const u8{ "es", "cs", "ss", "ds" })[(second >> 3) & 0b11],
        else => getGpRegisterName(@intCast(u3, (second >> 3) & 0b111), w),
    };

    try writer.writeByte(' ');
    if (d) try writer.print("{s}, ", .{reg_name});
    try writeModOperand(writer, reader, mod, rm, w, false);
    if (!d) try writer.print(", {s}", .{reg_name});
}

fn handleModSpecialInstr(reader: anytype, writer: anytype, first: u8, info: ModSpecialInstrInfo) !void {
    const w = first & 1 == 1;
    const mod = @intCast(u2, (info.byte >> 6) & 0b11);
    const rm = @intCast(u3, info.byte & 0b111);

    try writer.writeByte(' ');
    if (first == 0xff and (info.op == 3 or info.op == 5)) try writer.writeAll("FAR ");
    try writeModOperand(writer, reader, mod, rm, w, true);

    const operand_type: enum { imm, sx_imm8, one, cl, none } = switch (first) {
        0x83 => .sx_imm8,
        0x8f, 0xfe...0xff => .none,
        0xd0...0xd1 => .one,
        0xd2...0xd3 => .cl,
        0xf6...0xf7 => if (info.op == 0) .imm else .none,
        else => .imm,
    };
    switch (operand_type) {
        .imm => {
            const imm = if (w) try reader.readIntLittle(u16) else try reader.readByte();
            try writer.print(", 0x{x}", .{imm});
        },
        .sx_imm8 => {
            const imm = @bitCast(u16, @as(i16, try reader.readByteSigned()));
            try writer.print(", 0x{x}", .{imm});
        },
        .one => try writer.writeAll(", 1"),
        .cl => try writer.writeAll(", cl"),
        .none => {},
    }
}

fn writeModOperand(writer: anytype, reader: anytype, mod: u2, rm: u3, w: bool, show_ptrw: bool) !void {
    const ptrw = if (show_ptrw) w else null;
    switch (mod) {
        0 => {
            const disp = if (rm == 0b110) try reader.readIntLittle(u16) else 0;
            try writeModEffectiveAddr(writer, mod, rm, disp, ptrw);
        },
        1 => try writeModEffectiveAddr(writer, mod, rm, try reader.readByte(), ptrw),
        2 => try writeModEffectiveAddr(writer, mod, rm, try reader.readIntLittle(u16), ptrw),
        3 => try writer.writeAll(getGpRegisterName(rm, w)),
    }
}

fn writeModEffectiveAddr(writer: anytype, mod: u2, rm: u3, disp: u16, ptrw: ?bool) !void {
    if (ptrw) |w| {
        try writer.print("{s} PTR ", .{if (w) "WORD" else "BYTE"});
    }

    const reg_strs = [_][]const u8{ "bx+si", "bx+di", "bp+si", "bp+di", "si", "di", "bp", "bx" };
    switch (mod) {
        0 => if (rm == 0b110) {
            try writer.print("[0x{x}]", .{disp});
        } else {
            try writer.print("[{s}]", .{reg_strs[rm]});
        },
        1, 2 => try writer.print("[{s}+0x{x}]", .{ reg_strs[rm], disp }),
        3 => unreachable,
    }
}

fn handleAddrInstr(reader: anytype, writer: anytype, first: u8) !void {
    const w = first & 1 == 1;
    const d = (first >> 1) & 1 == 1;
    const addr = try reader.readIntLittle(u16);

    try writer.writeByte(' ');
    if (!d) try writer.print("{s}, ", .{getGpRegisterName(0, w)});
    try writer.print("[0x{x}]", .{addr});
    if (d) try writer.print(", {s}", .{getGpRegisterName(0, w)});
}

fn handleDataInstr(reader: anytype, writer: anytype, first: u8) !void {
    const info: struct { reg: u3, regw: bool, dataw: bool } = switch (first) {
        0xb0...0xbf => blk: {
            const w = (first >> 3) & 1 == 1;
            break :blk .{ .reg = @intCast(u3, first & 0b111), .regw = w, .dataw = w };
        },
        0xe4...0xe7 => .{ .reg = 0, .regw = first & 1 == 1, .dataw = false },
        else => blk: {
            const w = first & 1 == 1;
            break :blk .{ .reg = 0, .regw = w, .dataw = w };
        },
    };

    const reg_name = getGpRegisterName(info.reg, info.regw);
    const imm = if (info.dataw) try reader.readIntLittle(u16) else try reader.readByte();
    try writer.print(" {s}, 0x{x}", .{ reg_name, imm });
}

fn handleIpIncrInstr(reader: anytype, writer: anytype, first: u8) !void {
    const w = switch (first) {
        0xe8...0xe9 => true,
        else => false,
    };
    const off = if (w) try reader.readIntLittle(i16) else try reader.readByteSigned();
    try writer.print(" ${s}0x{x}", .{ if (off >= 0) "+" else "-", std.math.absCast(off) });
}

fn getGpRegisterName(reg: u3, w: bool) []const u8 {
    return ([_][8][]const u8{
        .{ "al", "cl", "dl", "bl", "ah", "ch", "dh", "bh" },
        .{ "ax", "cx", "dx", "bx", "sp", "bp", "si", "di" },
    })[@boolToInt(w)][reg];
}
