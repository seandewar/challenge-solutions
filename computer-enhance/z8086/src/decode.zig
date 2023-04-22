const std = @import("std");

const Operation = enum {
    aaa,
    aad,
    aam,
    aas,
    adc,
    add,
    @"and",
    call,
    cbw,
    clc,
    cld,
    cli,
    cmc,
    cmp,
    cmps,
    cwd,
    daa,
    das,
    dec,
    div,
    hlt,
    idiv,
    imul,
    in,
    inc,
    int,
    into,
    iret,
    jb,
    jbe,
    jcxz,
    je,
    jl,
    jle,
    jmp,
    jnb,
    jnbe,
    jne,
    jnl,
    jnle,
    jno,
    jnp,
    jns,
    jo,
    jp,
    js,
    lahf,
    lds,
    lea,
    les,
    lods,
    loop,
    loope,
    loopne,
    mov,
    movs,
    mul,
    neg,
    nop,
    not,
    @"or",
    out,
    pop,
    popf,
    push,
    pushf,
    rcl,
    rcr,
    rep,
    repne,
    ret,
    retf,
    rol,
    ror,
    sahf,
    sal,
    sar,
    sbb,
    scas,
    shr,
    stc,
    std,
    sti,
    stos,
    sub,
    @"test",
    wait,
    xchg,
    xlat,
    xor,

    lock_prefix,
    cs_prefix,
    ds_prefix,
    es_prefix,
    ss_prefix,
};

const Register = enum {
    // Byte registers.
    al,
    cl,
    dl,
    bl,
    ah,
    ch,
    dh,
    bh,

    // Wide registers.
    ax,
    cx,
    dx,
    bx,
    sp,
    bp,
    si,
    di,

    // Segment registers.
    es,
    cs,
    ss,
    ds,

    // In honour of gpanders! 🫡
    fn getGeneralPurpose(reg: u3, w: bool) Register {
        return @intToEnum(Register, @enumToInt(Register.ax) * @boolToInt(w) + reg);
    }

    fn getSegment(reg: u2) Register {
        return @intToEnum(Register, @enumToInt(Register.es) + reg);
    }
};

const Instruction = struct {
    op: Operation,
    payload: InstrPayload,
};

const InstrPayload = union(enum) {
    none,
    mod: ModInstr,
    mod_special: ModSpecialInstr,
    data: DataInstr,
    addr: AddrInstr,
    ip_disp: i16,
    interseg_addr: struct { cs: u16, addr: u16 },
    regs: struct { dst: Register, src: Register },
    reg: Register,
    imm: u16,
    dest_str8_src_str8,
    dest_str16_src_str16,
    dest_str8,
    dest_str16,
    src_str8,
    src_str16,
};

pub fn decodeNext(reader: anytype, error_payload: ?*UnknownInstrPayload) !?Instruction {
    const first = reader.readByte() catch |err| switch (err) {
        error.EndOfStream => return null,
        else => return err,
    };

    var mod_special_info: ModSpecialInstrInfo = undefined; // May need to read-ahead to identify these.
    const op: Operation = switch (first) {
        0x00...0x05 => .add,
        0x06, 0x0e, 0x16, 0x1e, 0x50...0x57 => .push,
        0x07, 0x17, 0x1f, 0x58...0x5f => .pop,
        0x08...0x0d => .@"or",
        0x10...0x15 => .adc,
        0x18...0x1d => .sbb,
        0x20...0x25 => .@"and",
        0x26 => .es_prefix,
        0x27 => .daa,
        0x28...0x2d => .sub,
        0x2e => .cs_prefix,
        0x2f => .das,
        0x30...0x35 => .xor,
        0x36 => .ss_prefix,
        0x37 => .aaa,
        0x38...0x3d => .cmp,
        0x3e => .ds_prefix,
        0x3f => .aas,
        0x40...0x47 => .inc,
        0x48...0x4f => .dec,

        0xd0...0xd3 => blk: {
            mod_special_info = try readModSpecialInstrInfo(reader);
            break :blk switch (mod_special_info.op) {
                0 => .rol,
                1 => .ror,
                2 => .rcl,
                3 => .rcr,
                4 => .sal,
                5 => .shr,
                7 => .sar,
                else => return unknownInstr(error_payload, first, mod_special_info),
            };
        },

        0xd4...0xd5 => blk: {
            mod_special_info = try readModSpecialInstrInfo(reader);
            if (mod_special_info.byte != 0b1010) return unknownInstr(error_payload, first, mod_special_info);
            break :blk if (first == 0xd4) .aam else .aad;
        },

        0x70 => .jo,
        0x71 => .jno,
        0x72 => .jb,
        0x73 => .jnb,
        0x74 => .je,
        0x75 => .jne,
        0x76 => .jbe,
        0x77 => .jnbe,
        0x78 => .js,
        0x79 => .jns,
        0x7a => .jp,
        0x7b => .jnp,
        0x7c => .jl,
        0x7d => .jnl,
        0x7e => .jle,
        0x7f => .jnle,

        0x80...0x83 => blk: {
            mod_special_info = try readModSpecialInstrInfo(reader);
            break :blk switch (mod_special_info.op) {
                0 => .add,
                1 => .@"or",
                2 => .adc,
                3 => .sbb,
                4 => .@"and",
                5 => .sub,
                6 => .xor,
                7 => .cmp,
            };
        },

        0x84...0x85, 0xa8...0xa9 => .@"test",
        0x86...0x87, 0x91...0x97 => .xchg,
        0x88...0x8c, 0x8e, 0xa0...0xa3, 0xb0...0xbf => .mov,

        0xc6...0xc7 => blk: {
            mod_special_info = try readModSpecialInstrInfo(reader);
            if (mod_special_info.op != 0) return unknownInstr(error_payload, first, mod_special_info);
            break :blk .mov;
        },

        0x8d => .lea,

        0x8f => blk: {
            mod_special_info = try readModSpecialInstrInfo(reader);
            if (mod_special_info.op != 0) return unknownInstr(error_payload, first, mod_special_info);
            break :blk .pop;
        },

        0x90 => .nop,
        0x98 => .cbw,
        0x99 => .cwd,
        0x9a, 0xe8 => .call,
        0x9b => .wait,
        0x9c => .pushf,
        0x9d => .popf,
        0x9e => .sahf,
        0x9f => .lahf,
        0xa4...0xa5 => .movs,
        0xa6...0xa7 => .cmps,
        0xaa...0xab => .stos,
        0xac...0xad => .lods,
        0xae...0xaf => .scas,
        0xc2...0xc3 => .ret,
        0xca...0xcb => .retf,
        0xc4 => .les,
        0xc5 => .lds,
        0xcc...0xcd => .int,
        0xce => .into,
        0xcf => .iret,
        0xd7 => .xlat,
        0xe0 => .loopne,
        0xe1 => .loope,
        0xe2 => .loop,
        0xe3 => .jcxz,
        0xe4...0xe5, 0xec...0xed => .in,
        0xe6...0xe7, 0xee...0xef => .out,
        0xe9...0xeb => .jmp,
        0xf0 => .lock_prefix,
        0xf2 => .repne,
        0xf3 => .rep,
        0xf4 => .hlt,
        0xf5 => .cmc,

        0xf6...0xf7 => blk: {
            mod_special_info = try readModSpecialInstrInfo(reader);
            break :blk switch (mod_special_info.op) {
                0 => .@"test",
                2 => .not,
                3 => .neg,
                4 => .mul,
                5 => .imul,
                6 => .div,
                7 => .idiv,
                else => return unknownInstr(error_payload, first, mod_special_info),
            };
        },

        0xf8 => .clc,
        0xf9 => .stc,
        0xfa => .cli,
        0xfb => .sti,
        0xfc => .cld,
        0xfd => .std,

        0xfe => blk: {
            mod_special_info = try readModSpecialInstrInfo(reader);
            break :blk switch (mod_special_info.op) {
                0 => .inc,
                1 => .dec,
                else => return unknownInstr(error_payload, first, mod_special_info),
            };
        },

        0xff => blk: {
            mod_special_info = try readModSpecialInstrInfo(reader);
            break :blk switch (mod_special_info.op) {
                0 => .inc,
                1 => .dec,
                2, 3 => .call,
                4, 5 => .jmp,
                6 => .push,
                else => return unknownInstr(error_payload, first, mod_special_info),
            };
        },

        else => return unknownInstr(error_payload, first, null),
    };

    const payload: InstrPayload = switch (first) {
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
        => .{ .mod = try decodeModInstr(reader, first) },

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
        => .{ .data = try decodeDataInstr(reader, first) },

        0x06...0x07 => .{ .reg = .es },
        0x0e => .{ .reg = .cs },
        0x16...0x17 => .{ .reg = .ss },
        0x1e...0x1f => .{ .reg = .ds },
        0x40...0x5f => .{ .reg = Register.getGeneralPurpose(@intCast(u3, first & 7), true) },

        0x70...0x7f, 0xe0...0xe3, 0xeb => .{ .ip_disp = try reader.readByteSigned() },
        0xe8...0xe9 => .{ .ip_disp = try reader.readIntLittle(i16) },

        0x80...0x83,
        0x8f,
        0xc6...0xc7,
        0xd0...0xd3,
        0xf6...0xf7,
        0xfe...0xff,
        => .{ .mod_special = try decodeModSpecialInstr(reader, first, mod_special_info) },

        0x91...0x97 => .{ .regs = .{
            .dst = .ax,
            .src = Register.getGeneralPurpose(@intCast(u3, first & 7), true),
        } },

        0x9a, 0xea => .{ .interseg_addr = .{
            .addr = try reader.readIntLittle(u16),
            .cs = try reader.readIntLittle(u16),
        } },

        0xa0...0xa3 => .{ .addr = try decodeAddrInstr(reader, first) },

        0xa4, 0xa6 => .dest_str8_src_str8,
        0xa5, 0xa7 => .dest_str16_src_str16,
        0xaa, 0xae => .dest_str8,
        0xab, 0xaf => .dest_str16,
        0xac => .src_str8,
        0xad => .src_str16,

        0xc2, 0xca => .{ .imm = try reader.readIntLittle(u16) },
        0xcc => .{ .imm = 3 },
        0xcd => .{ .imm = try reader.readByte() },

        0xec...0xef => .{ .regs = .{
            .dst = if (first & 1 == 1) .ax else .al,
            .src = .dx,
        } },

        else => .none,
    };

    return .{ .op = op, .payload = payload };
}

const UnknownInstrPayload = struct {
    first: u8,
    mod_spec_info: ?ModSpecialInstrInfo,
};

const ModSpecialInstrInfo = struct {
    byte: u8,
    op: u3,
};

fn readModSpecialInstrInfo(reader: anytype) !ModSpecialInstrInfo {
    const byte = try reader.readByte();
    return .{ .byte = byte, .op = @intCast(u3, (byte >> 3) & 0b111) };
}

fn unknownInstr(payload: ?*UnknownInstrPayload, first: u8, mod_spec_info: ?ModSpecialInstrInfo) error{UnknownInstr} {
    if (payload) |p| p.* = .{ .first = first, .mod_spec_info = mod_spec_info };
    return error.UnknownInstr;
}

const ModInstr = struct {
    d: bool,
    reg: Register,
    operand: ModOperand,

    fn getDst(self: ModInstr) ModOperand {
        return if (self.d) .{ .reg = self.reg } else self.operand;
    }

    fn getSrc(self: ModInstr) ModOperand {
        return if (self.d) self.operand else .{ .reg = self.reg };
    }
};

fn decodeModInstr(reader: anytype, first: u8) !ModInstr {
    const w = first & 1 == 1;
    const d = switch (first) {
        0x8d, 0xc4, 0xc5 => false,
        else => (first >> 1) & 1 == 1,
    };

    const second = try reader.readByte();
    const mod = @intCast(u2, (second >> 6) & 0b11);
    const rm = @intCast(u3, second & 0b111);
    const reg = switch (first) {
        0x8c, 0x8e => Register.getSegment(@intCast(u2, (second >> 3) & 0b11)),
        else => Register.getGeneralPurpose(@intCast(u3, (second >> 3) & 0b111), w),
    };
    const operand = try decodeModOperand(reader, mod, rm, w);

    return .{ .d = d, .reg = reg, .operand = operand };
}

const ModSpecialInstr = struct {
    dst: ModOperand,
    src: SrcOperand,

    const SrcOperand = union(enum) {
        none,
        cl,
        imm_unsigned: u16,
        imm_signed: i8,
    };
};

fn decodeModSpecialInstr(reader: anytype, first: u8, info: ModSpecialInstrInfo) !ModSpecialInstr {
    const w = first & 1 == 1;
    const mod = @intCast(u2, (info.byte >> 6) & 0b11);
    const rm = @intCast(u3, info.byte & 0b111);
    const dst = try decodeModOperand(reader, mod, rm, w);

    const src_type: @typeInfo(ModSpecialInstr.SrcOperand).Union.tag_type.? = switch (first) {
        0x83 => .imm_signed,
        0x8f, 0xfe, 0xff => .none,
        0xd2, 0xd3 => .cl,
        0xf6, 0xf7 => if (info.op == 0) .imm_unsigned else .none,
        else => .imm_unsigned,
    };
    const src: ModSpecialInstr.SrcOperand = switch (src_type) {
        .imm_unsigned => switch (first) {
            0xd0, 0xd1 => .{ .imm_unsigned = 1 },
            else => .{ .imm_unsigned = if (w) try reader.readIntLittle(u16) else try reader.readByte() },
        },
        .imm_signed => .{ .imm_signed = try reader.readByteSigned() },
        inline else => |tag| tag,
    };

    return .{ .dst = dst, .src = src };
}

const ModOperand = union(enum) {
    reg: Register,
    addr: EffectiveAddr,
};

fn decodeModOperand(reader: anytype, mod: u2, rm: u3, w: bool) !ModOperand {
    return switch (mod) {
        0 => .{ .addr = decodeModEffectiveAddr(mod, rm, if (rm == 0b110) try reader.readIntLittle(u16) else 0) },
        1 => .{ .addr = decodeModEffectiveAddr(mod, rm, try reader.readByte()) },
        2 => .{ .addr = decodeModEffectiveAddr(mod, rm, try reader.readIntLittle(u16)) },
        3 => .{ .reg = Register.getGeneralPurpose(rm, w) },
    };
}

const EffectiveAddr = struct {
    disp: u16 = 0,
    disp_regs: DispRegisters = .none,

    const DispRegisters = enum {
        bx_si,
        bx_di,
        bp_si,
        bp_di,
        si,
        di,
        bp,
        bx,
        none,
    };
};

fn decodeModEffectiveAddr(mod: u2, rm: u3, disp: u16) EffectiveAddr {
    return switch (mod) {
        0 => return if (rm == 0b110)
            .{ .disp = disp }
        else
            .{ .disp_regs = @intToEnum(EffectiveAddr.DispRegisters, rm) },
        1, 2 => .{ .disp_regs = @intToEnum(EffectiveAddr.DispRegisters, rm), .disp = disp },
        3 => unreachable,
    };
}

const DataInstr = struct {
    reg: Register,
    imm: u16,
};

fn decodeDataInstr(reader: anytype, first: u8) !DataInstr {
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

    return .{
        .reg = Register.getGeneralPurpose(info.reg, info.regw),
        .imm = if (info.dataw) try reader.readIntLittle(u16) else try reader.readByte(),
    };
}

const AddrInstr = struct {
    d: bool,
    reg: Register,
    addr: u16,

    const Operand = union(enum) {
        reg: Register,
        addr: u16,
    };

    fn getDst(self: AddrInstr) Operand {
        return if (self.d) .{ .addr = self.addr } else .{ .reg = self.reg };
    }

    fn getSrc(self: AddrInstr) Operand {
        return if (self.d) .{ .reg = self.reg } else .{ .addr = self.addr };
    }
};

fn decodeAddrInstr(reader: anytype, first: u8) !AddrInstr {
    const w = first & 1 == 1;
    const d = (first >> 1) & 1 == 1;
    const addr = try reader.readIntLittle(u16);
    return .{ .d = d, .reg = if (w) .ax else .al, .addr = addr };
}

// Following tests are based on the "Computer, Enhance!" perfaware listings:
// https://github.com/cmuratori/computer_enhance/blob/main/perfaware/part1
const testing = std.testing;

test "37: single register mov" {
    var stream = std.io.fixedBufferStream("\x89\xd9");
    const reader = stream.reader();

    try testExpectModInstr(.mov, .{ .reg = .cx }, .{ .reg = .bx }, try decodeNext(reader, null));
    try testing.expectEqual(@as(?Instruction, null), try decodeNext(reader, null));
}

test "38: many register mov" {
    var stream = std.io.fixedBufferStream(
        "\x89\xd9\x88\xe5\x89\xda\x89\xde\x89\xfb\x88\xc8\x88\xed\x89\xc3" ++
            "\x89\xf3\x89\xfc\x89\xc5",
    );
    const reader = stream.reader();

    try testExpectModInstr(.mov, .{ .reg = .cx }, .{ .reg = .bx }, try decodeNext(reader, null));
    try testExpectModInstr(.mov, .{ .reg = .ch }, .{ .reg = .ah }, try decodeNext(reader, null));
    try testExpectModInstr(.mov, .{ .reg = .dx }, .{ .reg = .bx }, try decodeNext(reader, null));
    try testExpectModInstr(.mov, .{ .reg = .si }, .{ .reg = .bx }, try decodeNext(reader, null));
    try testExpectModInstr(.mov, .{ .reg = .bx }, .{ .reg = .di }, try decodeNext(reader, null));
    try testExpectModInstr(.mov, .{ .reg = .al }, .{ .reg = .cl }, try decodeNext(reader, null));
    try testExpectModInstr(.mov, .{ .reg = .ch }, .{ .reg = .ch }, try decodeNext(reader, null));
    try testExpectModInstr(.mov, .{ .reg = .bx }, .{ .reg = .ax }, try decodeNext(reader, null));
    try testExpectModInstr(.mov, .{ .reg = .bx }, .{ .reg = .si }, try decodeNext(reader, null));
    try testExpectModInstr(.mov, .{ .reg = .sp }, .{ .reg = .di }, try decodeNext(reader, null));
    try testExpectModInstr(.mov, .{ .reg = .bp }, .{ .reg = .ax }, try decodeNext(reader, null));
    try testing.expectEqual(@as(?Instruction, null), try decodeNext(reader, null));
}

test "39: more movs" {
    var stream = std.io.fixedBufferStream(
        "\x89\xde\x88\xc6\xb1\x0c\xb5\xf4\xb9\x0c\x00\xb9\xf4\xff\xba\x6c" ++
            "\x0f\xba\x94\xf0\x8a\x00\x8b\x1b\x8b\x56\x00\x8a\x60\x04\x8a\x80" ++
            "\x87\x13\x89\x09\x88\x0a\x88\x6e\x00",
    );
    const reader = stream.reader();

    // Register-to-register.
    try testExpectModInstr(.mov, .{ .reg = .si }, .{ .reg = .bx }, try decodeNext(reader, null));
    try testExpectModInstr(.mov, .{ .reg = .dh }, .{ .reg = .al }, try decodeNext(reader, null));

    // 8-bit intermediate-to-register.
    try testExpectDataInstr(.mov, .cl, 12, try decodeNext(reader, null));
    try testExpectDataInstr(.mov, .ch, @bitCast(u8, @as(i8, -12)), try decodeNext(reader, null));

    // 16-bit intermediate-to-register.
    try testExpectDataInstr(.mov, .cx, 12, try decodeNext(reader, null));
    try testExpectDataInstr(.mov, .cx, @bitCast(u16, @as(i16, -12)), try decodeNext(reader, null));
    try testExpectDataInstr(.mov, .dx, 3948, try decodeNext(reader, null));
    try testExpectDataInstr(.mov, .dx, @bitCast(u16, @as(i16, -3948)), try decodeNext(reader, null));

    // Source address calculation.
    try testExpectModInstr(.mov, .{ .reg = .al }, .{ .addr = .{ .disp_regs = .bx_si } }, try decodeNext(reader, null));
    try testExpectModInstr(.mov, .{ .reg = .bx }, .{ .addr = .{ .disp_regs = .bp_di } }, try decodeNext(reader, null));
    try testExpectModInstr(.mov, .{ .reg = .dx }, .{ .addr = .{ .disp_regs = .bp } }, try decodeNext(reader, null));

    // Source address calculation plus 8-bit displacement.
    try testExpectModInstr(
        .mov,
        .{ .reg = .ah },
        .{ .addr = .{ .disp_regs = .bx_si, .disp = 4 } },
        try decodeNext(reader, null),
    );

    // Source address calculation plus 16-bit displacement.
    try testExpectModInstr(
        .mov,
        .{ .reg = .al },
        .{ .addr = .{ .disp_regs = .bx_si, .disp = 4999 } },
        try decodeNext(reader, null),
    );

    // Destination address calculation.
    try testExpectModInstr(.mov, .{ .addr = .{ .disp_regs = .bx_di } }, .{ .reg = .cx }, try decodeNext(reader, null));
    try testExpectModInstr(.mov, .{ .addr = .{ .disp_regs = .bp_si } }, .{ .reg = .cl }, try decodeNext(reader, null));
    try testExpectModInstr(.mov, .{ .addr = .{ .disp_regs = .bp } }, .{ .reg = .ch }, try decodeNext(reader, null));

    try testing.expectEqual(@as(?Instruction, null), try decodeNext(reader, null));
}

test "40: challenge movs" {
    var stream = std.io.fixedBufferStream(
        "\x8b\x41\xdb\x89\x8c\xd4\xfe\x8b\x57\xe0\xc6\x03\x07\xc7\x85\x85" ++
            "\x03\x5b\x01\x8b\x2e\x05\x00\x8b\x1e\x82\x0d\xa1\xfb\x09\xa1\x10" ++
            "\x00\xa3\xfa\x09\xa3\x0f\x00",
    );
    const reader = stream.reader();

    // Signed displacements.
    try testExpectModInstr(
        .mov,
        .{ .reg = .ax },
        .{ .addr = .{ .disp_regs = .bx_di, .disp = @bitCast(u8, @as(i8, -37)) } },
        try decodeNext(reader, null),
    );
    try testExpectModInstr(
        .mov,
        .{ .addr = .{ .disp_regs = .si, .disp = @bitCast(u16, @as(i16, -300)) } },
        .{ .reg = .cx },
        try decodeNext(reader, null),
    );
    try testExpectModInstr(
        .mov,
        .{ .reg = .dx },
        .{ .addr = .{ .disp_regs = .bx, .disp = @bitCast(u8, @as(i8, -32)) } },
        try decodeNext(reader, null),
    );

    // Explicit sizes.
    try testExpectModSpecialInstr(
        .mov,
        .{ .addr = .{ .disp_regs = .bp_di } },
        .{ .imm_unsigned = 7 },
        try decodeNext(reader, null),
    );
    try testExpectModSpecialInstr(
        .mov,
        .{ .addr = .{ .disp_regs = .di, .disp = 901 } },
        .{ .imm_unsigned = 347 },
        try decodeNext(reader, null),
    );

    // Direct address.
    try testExpectModInstr(.mov, .{ .reg = .bp }, .{ .addr = .{ .disp = 5 } }, try decodeNext(reader, null));
    try testExpectModInstr(.mov, .{ .reg = .bx }, .{ .addr = .{ .disp = 3458 } }, try decodeNext(reader, null));

    // Memory-to-accumulator test.
    try testExpectAddrInstr(.mov, .{ .reg = .ax }, .{ .addr = 2555 }, try decodeNext(reader, null));
    try testExpectAddrInstr(.mov, .{ .reg = .ax }, .{ .addr = 16 }, try decodeNext(reader, null));

    // Accumulator-to-memory test.
    try testExpectAddrInstr(.mov, .{ .addr = 2554 }, .{ .reg = .ax }, try decodeNext(reader, null));
    try testExpectAddrInstr(.mov, .{ .addr = 15 }, .{ .reg = .ax }, try decodeNext(reader, null));

    try testing.expectEqual(@as(?Instruction, null), try decodeNext(reader, null));
}

test "41: add sub cmp jnz" {
    var stream = std.io.fixedBufferStream(
        "\x03\x18\x03\x5e\x00\x83\xc6\x02\x83\xc5\x02\x83\xc1\x08\x03\x5e" ++
            "\x00\x03\x4f\x02\x02\x7a\x04\x03\x7b\x06\x01\x18\x01\x5e\x00\x01" ++
            "\x5e\x00\x01\x4f\x02\x00\x7a\x04\x01\x7b\x06\x80\x07\x22\x83\x82" ++
            "\xe8\x03\x1d\x03\x46\x00\x02\x00\x01\xd8\x00\xe0\x05\xe8\x03\x04" ++
            "\xe2\x04\x09\x2b\x18\x2b\x5e\x00\x83\xee\x02\x83\xed\x02\x83\xe9" ++
            "\x08\x2b\x5e\x00\x2b\x4f\x02\x2a\x7a\x04\x2b\x7b\x06\x29\x18\x29" ++
            "\x5e\x00\x29\x5e\x00\x29\x4f\x02\x28\x7a\x04\x29\x7b\x06\x80\x2f" ++
            "\x22\x83\x29\x1d\x2b\x46\x00\x2a\x00\x29\xd8\x28\xe0\x2d\xe8\x03" ++
            "\x2c\xe2\x2c\x09\x3b\x18\x3b\x5e\x00\x83\xfe\x02\x83\xfd\x02\x83" ++
            "\xf9\x08\x3b\x5e\x00\x3b\x4f\x02\x3a\x7a\x04\x3b\x7b\x06\x39\x18" ++
            "\x39\x5e\x00\x39\x5e\x00\x39\x4f\x02\x38\x7a\x04\x39\x7b\x06\x80" ++
            "\x3f\x22\x83\x3e\xe2\x12\x1d\x3b\x46\x00\x3a\x00\x39\xd8\x38\xe0" ++
            "\x3d\xe8\x03\x3c\xe2\x3c\x09\x75\x02\x75\xfc\x75\xfa\x75\xfc\x74" ++
            "\xfe\x7c\xfc\x7e\xfa\x72\xf8\x76\xf6\x7a\xf4\x70\xf2\x78\xf0\x75" ++
            "\xee\x7d\xec\x7f\xea\x73\xe8\x77\xe6\x7b\xe4\x71\xe2\x79\xe0\xe2" ++
            "\xde\xe1\xdc\xe0\xda\xe3\xd8",
    );
    const reader = stream.reader();

    for ([_]Operation{ .add, .sub, .cmp }) |op| {
        try testExpectModInstr(op, .{ .reg = .bx }, .{ .addr = .{ .disp_regs = .bx_si } }, try decodeNext(reader, null));
        try testExpectModInstr(op, .{ .reg = .bx }, .{ .addr = .{ .disp_regs = .bp } }, try decodeNext(reader, null));
        try testExpectModSpecialInstr(op, .{ .reg = .si }, .{ .imm_signed = 2 }, try decodeNext(reader, null));
        try testExpectModSpecialInstr(op, .{ .reg = .bp }, .{ .imm_signed = 2 }, try decodeNext(reader, null));
        try testExpectModSpecialInstr(op, .{ .reg = .cx }, .{ .imm_signed = 8 }, try decodeNext(reader, null));
        try testExpectModInstr(op, .{ .reg = .bx }, .{ .addr = .{ .disp_regs = .bp } }, try decodeNext(reader, null));
        try testExpectModInstr(op, .{ .reg = .cx }, .{ .addr = .{ .disp_regs = .bx, .disp = 2 } }, try decodeNext(reader, null));
        try testExpectModInstr(op, .{ .reg = .bh }, .{ .addr = .{ .disp_regs = .bp_si, .disp = 4 } }, try decodeNext(reader, null));
        try testExpectModInstr(op, .{ .reg = .di }, .{ .addr = .{ .disp_regs = .bp_di, .disp = 6 } }, try decodeNext(reader, null));
        try testExpectModInstr(op, .{ .addr = .{ .disp_regs = .bx_si } }, .{ .reg = .bx }, try decodeNext(reader, null));
        try testExpectModInstr(op, .{ .addr = .{ .disp_regs = .bp } }, .{ .reg = .bx }, try decodeNext(reader, null));
        try testExpectModInstr(op, .{ .addr = .{ .disp_regs = .bp } }, .{ .reg = .bx }, try decodeNext(reader, null));
        try testExpectModInstr(op, .{ .addr = .{ .disp_regs = .bx, .disp = 2 } }, .{ .reg = .cx }, try decodeNext(reader, null));
        try testExpectModInstr(op, .{ .addr = .{ .disp_regs = .bp_si, .disp = 4 } }, .{ .reg = .bh }, try decodeNext(reader, null));
        try testExpectModInstr(op, .{ .addr = .{ .disp_regs = .bp_di, .disp = 6 } }, .{ .reg = .di }, try decodeNext(reader, null));
        try testExpectModSpecialInstr(op, .{ .addr = .{ .disp_regs = .bx } }, .{ .imm_unsigned = 34 }, try decodeNext(reader, null));

        {
            const addr: EffectiveAddr = switch (op) {
                .add => .{ .disp_regs = .bp_si, .disp = 1000 },
                .sub => .{ .disp_regs = .bx_di },
                .cmp => .{ .disp = 4834 },
                else => unreachable,
            };
            try testExpectModSpecialInstr(op, .{ .addr = addr }, .{ .imm_signed = 29 }, try decodeNext(reader, null));
        }

        try testExpectModInstr(op, .{ .reg = .ax }, .{ .addr = .{ .disp_regs = .bp } }, try decodeNext(reader, null));
        try testExpectModInstr(op, .{ .reg = .al }, .{ .addr = .{ .disp_regs = .bx_si } }, try decodeNext(reader, null));
        try testExpectModInstr(op, .{ .reg = .ax }, .{ .reg = .bx }, try decodeNext(reader, null));
        try testExpectModInstr(op, .{ .reg = .al }, .{ .reg = .ah }, try decodeNext(reader, null));
        try testExpectDataInstr(op, .ax, 1000, try decodeNext(reader, null));
        try testExpectDataInstr(op, .al, @bitCast(u8, @as(i8, -30)), try decodeNext(reader, null));
        try testExpectDataInstr(op, .al, 9, try decodeNext(reader, null));
    }

    try testing.expectEqual(Instruction{ .op = .jne, .payload = .{ .ip_disp = 2 } }, (try decodeNext(reader, null)).?);
    try testing.expectEqual(Instruction{ .op = .jne, .payload = .{ .ip_disp = -4 } }, (try decodeNext(reader, null)).?);
    try testing.expectEqual(Instruction{ .op = .jne, .payload = .{ .ip_disp = -6 } }, (try decodeNext(reader, null)).?);
    try testing.expectEqual(Instruction{ .op = .jne, .payload = .{ .ip_disp = -4 } }, (try decodeNext(reader, null)).?);

    inline for (.{
        .je,
        .jl,
        .jle,
        .jb,
        .jbe,
        .jp,
        .jo,
        .js,
        .jne,
        .jnl,
        .jnle,
        .jnb,
        .jnbe,
        .jnp,
        .jno,
        .jns,
        .loop,
        .loope,
        .loopne,
        .jcxz,
    }, 1..) |op, off| {
        try testing.expectEqual(
            Instruction{ .op = op, .payload = .{ .ip_disp = -2 * @intCast(i16, off) } },
            (try decodeNext(reader, null)).?,
        );
    }

    try testing.expectEqual(@as(?Instruction, null), try decodeNext(reader, null));
}

test "42: completionist decode" {
    var stream = std.io.fixedBufferStream(
        "\x89\xde\x88\xc6\xb1\x0c\xb5\xf4\xb9\x0c\x00\xb9\xf4\xff\xba\x6c" ++
            "\x0f\xba\x94\xf0\x8a\x00\x8b\x1b\x8b\x56\x00\x8a\x60\x04\x8a\x80" ++
            "\x87\x13\x89\x09\x88\x0a\x88\x6e\x00\x8b\x41\xdb\x89\x8c\xd4\xfe" ++
            "\x8b\x57\xe0\xc6\x03\x07\xc7\x85\x85\x03\x5b\x01\x8b\x2e\x05\x00" ++
            "\x8b\x1e\x82\x0d\xa1\xfb\x09\xa1\x10\x00\xa3\xfa\x09\xa3\x0f\x00" ++
            "\xff\x32\xff\x36\xb8\x0b\xff\x71\xe2\x51\x50\x52\x0e\x8f\x02\x8f" ++
            "\x06\x03\x00\x8f\x81\x48\xf4\x5c\x5f\x5e\x1f\x87\x86\x18\xfc\x87" ++
            "\x6f\x32\x90\x92\x94\x96\x97\x87\xca\x87\xf1\x86\xcc\xe4\xc8\xec" ++
            "\xed\xe7\x2c\xee\xd7\x8d\x81\x8c\x05\x8d\x5e\xce\x8d\xa6\x15\xfc" ++
            "\x8d\x78\xf9\xc5\x81\x8c\x05\xc5\x5e\xce\xc5\xa6\x15\xfc\xc5\x78" ++
            "\xf9\xc4\x81\x8c\x05\xc4\x5e\xce\xc4\xa6\x15\xfc\xc4\x78\xf9\x9f" ++
            "\x9e\x9c\x9d\x03\x4e\x00\x03\x10\x00\xa3\x88\x13\x00\x07\x81\xc4" ++
            "\x88\x01\x83\xc6\x05\x05\xe8\x03\x80\xc4\x1e\x04\x09\x01\xd9\x00" ++
            "\xc5\x13\x4e\x00\x13\x10\x10\xa3\x88\x13\x10\x07\x81\xd4\x88\x01" ++
            "\x83\xd6\x05\x15\xe8\x03\x80\xd4\x1e\x14\x09\x11\xd9\x10\xc5\x40" ++
            "\x41\xfe\xc6\xfe\xc0\xfe\xc4\x44\x47\xfe\x86\xea\x03\xff\x47\x27" ++
            "\xfe\x40\x05\xff\x83\xc4\xd8\xff\x06\x85\x24\xfe\x46\x00\x37\x27" ++
            "\x2b\x4e\x00\x2b\x10\x28\xa3\x88\x13\x28\x07\x81\xec\x88\x01\x83" ++
            "\xee\x05\x2d\xe8\x03\x80\xec\x1e\x2c\x09\x29\xd9\x28\xc5\x1b\x4e" ++
            "\x00\x1b\x10\x18\xa3\x88\x13\x18\x07\x81\xdc\x88\x01\x83\xde\x05" ++
            "\x1d\xe8\x03\x80\xdc\x1e\x1c\x09\x19\xd9\x18\xc5\x48\x49\xfe\xce" ++
            "\xfe\xc8\xfe\xcc\x4c\x4f\xfe\x8e\xea\x03\xff\x4f\x27\xfe\x48\x05" ++
            "\xff\x8b\xc4\xd8\xff\x0e\x85\x24\xfe\x4e\x00\xf7\xd8\xf7\xd9\xf6" ++
            "\xde\xf6\xd8\xf6\xdc\xf7\xdc\xf7\xdf\xf6\x9e\xea\x03\xf7\x5f\x27" ++
            "\xf6\x58\x05\xf7\x9b\xc4\xd8\xf7\x1e\x85\x24\xf6\x5e\x00\x39\xcb" ++
            "\x3a\xb6\x86\x01\x39\x76\x02\x80\xfb\x14\x80\x3f\x22\x3d\x65\x5d" ++
            "\x3f\x2f\xf6\xe0\xf7\xe1\xf7\x66\x00\xf6\xa1\xf4\x01\xf6\xed\xf7" ++
            "\xea\xf6\x2f\xf7\x2e\x0b\x25\xd4\x0a\xf6\xf3\xf7\xf4\xf6\xb0\xae" ++
            "\x0b\xf7\xb3\xe8\x03\xf7\xf8\xf7\xfe\xf6\x3a\xf7\xbf\xed\x01\xd5" ++
            "\x0a\x98\x99\xf6\xd4\xf6\xd3\xf7\xd4\xf7\xd6\xf7\x56\x00\xf6\x96" ++
            "\xb1\x26\xd0\xe4\xd1\xe8\xd1\xfb\xd1\xc1\xd0\xce\xd1\xd4\xd1\xdd" ++
            "\xd1\x66\x05\xd0\xa8\x39\xff\xd0\xb9\xd4\xfe\xd1\x46\x00\xd1\x0e" ++
            "\x4a\x13\xd0\x16\x03\x00\xd1\x1f\xd2\xe4\xd3\xe8\xd3\xfb\xd3\xc1" ++
            "\xd2\xce\xd3\xd4\xd3\xdd\xd3\x66\x05\xd3\xa8\x39\xff\xd2\xb9\xd4" ++
            "\xfe\xd2\x46\x00\xd2\x0e\x4a\x13\xd2\x16\x03\x00\xd3\x1f\x20\xe0" ++
            "\x20\xcd\x21\xf5\x21\xe7\x24\x5d\x25\xa8\x4f\x20\x6a\x0a\x21\x91" ++
            "\xe8\x03\x23\x5e\x00\x23\x0e\x20\x11\x80\x66\xd9\xef\x81\xa0\x14" ++
            "\xef\x58\x28\x85\xcb\x84\xb6\x86\x01\x85\x76\x02\xf6\xc3\x14\xf6" ++
            "\x07\x22\xa9\x65\x5d\x08\xe0\x08\xcd\x09\xf5\x09\xe7\x0c\x5d\x0d" ++
            "\xa8\x4f\x08\x6a\x0a\x09\x91\xe8\x03\x0b\x5e\x00\x0b\x0e\x20\x11" ++
            "\x80\x4e\xd9\xef\x81\x88\x14\xef\x58\x28\x30\xe0\x30\xcd\x31\xf5" ++
            "\x31\xe7\x34\x5d\x35\xa8\x4f\x30\x6a\x0a\x31\x91\xe8\x03\x33\x5e" ++
            "\x00\x33\x0e\x20\x11\x80\x76\xd9\xef\x81\xb0\x14\xef\x58\x28\xf3" ++
            "\xa4\xf3\xa6\xf3\xae\xf3\xac\xf3\xa5\xf3\xa7\xf3\xaf\xf3\xad\xf3" ++
            "\xaa\xf3\xab\xff\x16\x21\x99\xff\x56\x9c\xff\xd4\xff\xd0\xff\xe0" ++
            "\xff\xe7\xff\x26\x0c\x00\xff\x26\x2b\x11\xc2\xf9\xff\xc2\xf4\x01" ++
            "\xc3\x74\xfe\x7c\xfc\x7e\xfa\x72\xf8\x76\xf6\x7a\xf4\x70\xf2\x78" ++
            "\xf0\x75\xee\x7d\xec\x7f\xea\x73\xe8\x77\xe6\x7b\xe4\x71\xe2\x79" ++
            "\xe0\xe2\xde\xe1\xdc\xe0\xda\xe3\xd8\xcd\x0d\xcc\xce\xcf\xf8\xf5" ++
            "\xf9\xfc\xfd\xfa\xfb\xf4\x9b\xf0\xf6\x96\xb1\x26\xf0\x86\x06\x64" ++
            "\x00\x2e\x8a\x00\x3e\x8b\x1b\x26\x8b\x56\x00\x36\x8a\x60\x04\x36" ++
            "\x20\x6a\x0a\x3e\x09\x91\xe8\x03\x26\x33\x5e\x00\x26\x3b\x0e\x20" ++
            "\x11\x2e\xf6\x46\xd9\xef\x2e\x81\x98\x14\xef\x58\x28\xf0\x2e\xf6" ++
            "\x96\xb1\x26\x9a\xc8\x01\x7b\x00\xea\x22\x00\x15\x03\x8c\x40\x3b" ++
            "\xe9\xd9\x06\xe8\xb6\x2a\xca\x94\x44\xc2\x98\x44\xcb\xc3\xff\x52" ++
            "\xc6\xff\x5a\xc6\xff\x25\xff\x2d\xea\x88\x77\x66\x55",
    );
    const reader = stream.reader();

    try testExpectModInstr(.mov, .{ .reg = .si }, .{ .reg = .bx }, try decodeNext(reader, null));
    try testExpectModInstr(.mov, .{ .reg = .dh }, .{ .reg = .al }, try decodeNext(reader, null));
    try testExpectDataInstr(.mov, .cl, 12, try decodeNext(reader, null));
    try testExpectDataInstr(.mov, .ch, @bitCast(u8, @as(i8, -12)), try decodeNext(reader, null));
    try testExpectDataInstr(.mov, .cx, 12, try decodeNext(reader, null));
    try testExpectDataInstr(.mov, .cx, @bitCast(u16, @as(i16, -12)), try decodeNext(reader, null));
    try testExpectDataInstr(.mov, .dx, 3948, try decodeNext(reader, null));
    try testExpectDataInstr(.mov, .dx, @bitCast(u16, @as(i16, -3948)), try decodeNext(reader, null));
    try testExpectModInstr(.mov, .{ .reg = .al }, .{ .addr = .{ .disp_regs = .bx_si } }, try decodeNext(reader, null));
    try testExpectModInstr(.mov, .{ .reg = .bx }, .{ .addr = .{ .disp_regs = .bp_di } }, try decodeNext(reader, null));
    try testExpectModInstr(.mov, .{ .reg = .dx }, .{ .addr = .{ .disp_regs = .bp } }, try decodeNext(reader, null));
    try testExpectModInstr(.mov, .{ .reg = .ah }, .{ .addr = .{ .disp_regs = .bx_si, .disp = 4 } }, try decodeNext(reader, null));
    try testExpectModInstr(.mov, .{ .reg = .al }, .{ .addr = .{ .disp_regs = .bx_si, .disp = 4999 } }, try decodeNext(reader, null));
    try testExpectModInstr(.mov, .{ .addr = .{ .disp_regs = .bx_di } }, .{ .reg = .cx }, try decodeNext(reader, null));
    try testExpectModInstr(.mov, .{ .addr = .{ .disp_regs = .bp_si } }, .{ .reg = .cl }, try decodeNext(reader, null));
    try testExpectModInstr(.mov, .{ .addr = .{ .disp_regs = .bp } }, .{ .reg = .ch }, try decodeNext(reader, null));
    try testExpectModInstr(
        .mov,
        .{ .reg = .ax },
        .{ .addr = .{ .disp_regs = .bx_di, .disp = @bitCast(u8, @as(i8, -37)) } },
        try decodeNext(reader, null),
    );
    try testExpectModInstr(
        .mov,
        .{ .addr = .{ .disp_regs = .si, .disp = @bitCast(u16, @as(i16, -300)) } },
        .{ .reg = .cx },
        try decodeNext(reader, null),
    );
    try testExpectModInstr(
        .mov,
        .{ .reg = .dx },
        .{ .addr = .{ .disp_regs = .bx, .disp = @bitCast(u8, @as(i8, -32)) } },
        try decodeNext(reader, null),
    );
    try testExpectModSpecialInstr(
        .mov,
        .{ .addr = .{ .disp_regs = .bp_di } },
        .{ .imm_unsigned = 7 },
        try decodeNext(reader, null),
    );
    try testExpectModSpecialInstr(
        .mov,
        .{ .addr = .{ .disp_regs = .di, .disp = 901 } },
        .{ .imm_unsigned = 347 },
        try decodeNext(reader, null),
    );
    try testExpectModInstr(.mov, .{ .reg = .bp }, .{ .addr = .{ .disp = 5 } }, try decodeNext(reader, null));
    try testExpectModInstr(.mov, .{ .reg = .bx }, .{ .addr = .{ .disp = 3458 } }, try decodeNext(reader, null));
    try testExpectAddrInstr(.mov, .{ .reg = .ax }, .{ .addr = 2555 }, try decodeNext(reader, null));
    try testExpectAddrInstr(.mov, .{ .reg = .ax }, .{ .addr = 16 }, try decodeNext(reader, null));
    try testExpectAddrInstr(.mov, .{ .addr = 2554 }, .{ .reg = .ax }, try decodeNext(reader, null));
    try testExpectAddrInstr(.mov, .{ .addr = 15 }, .{ .reg = .ax }, try decodeNext(reader, null));

    try testExpectModSpecialInstr(.push, .{ .addr = .{ .disp_regs = .bp_si } }, .none, try decodeNext(reader, null));
    try testExpectModSpecialInstr(.push, .{ .addr = .{ .disp = 3000 } }, .none, try decodeNext(reader, null));
    try testExpectModSpecialInstr(
        .push,
        .{ .addr = .{ .disp_regs = .bx_di, .disp = @bitCast(u8, @as(i8, -30)) } },
        .none,
        try decodeNext(reader, null),
    );
    try testing.expectEqual(Instruction{ .op = .push, .payload = .{ .reg = .cx } }, (try decodeNext(reader, null)).?);
    try testing.expectEqual(Instruction{ .op = .push, .payload = .{ .reg = .ax } }, (try decodeNext(reader, null)).?);
    try testing.expectEqual(Instruction{ .op = .push, .payload = .{ .reg = .dx } }, (try decodeNext(reader, null)).?);
    try testing.expectEqual(Instruction{ .op = .push, .payload = .{ .reg = .cs } }, (try decodeNext(reader, null)).?);

    try testExpectModSpecialInstr(.pop, .{ .addr = .{ .disp_regs = .bp_si } }, .none, try decodeNext(reader, null));
    try testExpectModSpecialInstr(.pop, .{ .addr = .{ .disp = 3 } }, .none, try decodeNext(reader, null));
    try testExpectModSpecialInstr(
        .pop,
        .{ .addr = .{ .disp_regs = .bx_di, .disp = @bitCast(u16, @as(i16, -3000)) } },
        .none,
        try decodeNext(reader, null),
    );
    try testing.expectEqual(Instruction{ .op = .pop, .payload = .{ .reg = .sp } }, (try decodeNext(reader, null)).?);
    try testing.expectEqual(Instruction{ .op = .pop, .payload = .{ .reg = .di } }, (try decodeNext(reader, null)).?);
    try testing.expectEqual(Instruction{ .op = .pop, .payload = .{ .reg = .si } }, (try decodeNext(reader, null)).?);
    try testing.expectEqual(Instruction{ .op = .pop, .payload = .{ .reg = .ds } }, (try decodeNext(reader, null)).?);

    try testExpectModInstr(
        .xchg,
        .{ .reg = .ax },
        .{ .addr = .{ .disp_regs = .bp, .disp = @bitCast(u16, @as(i16, -1000)) } },
        try decodeNext(reader, null),
    );
    try testExpectModInstr(.xchg, .{ .reg = .bp }, .{ .addr = .{ .disp_regs = .bx, .disp = 50 } }, try decodeNext(reader, null));

    try testing.expectEqual(Instruction{ .op = .nop, .payload = .none }, (try decodeNext(reader, null)).?);
    try testing.expectEqual(
        Instruction{ .op = .xchg, .payload = .{ .regs = .{ .dst = .ax, .src = .dx } } },
        (try decodeNext(reader, null)).?,
    );
    try testing.expectEqual(
        Instruction{ .op = .xchg, .payload = .{ .regs = .{ .dst = .ax, .src = .sp } } },
        (try decodeNext(reader, null)).?,
    );
    try testing.expectEqual(
        Instruction{ .op = .xchg, .payload = .{ .regs = .{ .dst = .ax, .src = .si } } },
        (try decodeNext(reader, null)).?,
    );
    try testing.expectEqual(
        Instruction{ .op = .xchg, .payload = .{ .regs = .{ .dst = .ax, .src = .di } } },
        (try decodeNext(reader, null)).?,
    );

    try testExpectModInstr(.xchg, .{ .reg = .cx }, .{ .reg = .dx }, try decodeNext(reader, null));
    try testExpectModInstr(.xchg, .{ .reg = .si }, .{ .reg = .cx }, try decodeNext(reader, null));
    try testExpectModInstr(.xchg, .{ .reg = .cl }, .{ .reg = .ah }, try decodeNext(reader, null));

    // TODO
    std.debug.print(
        "TODO next instr: {}\n",
        .{std.fmt.Formatter(struct {
            fn f(instr: Instruction, comptime fmt: []const u8, options: std.fmt.FormatOptions, writer: anytype) !void {
                return std.fmt.formatType(instr, fmt, options, writer, 999);
            }
        }.f){ .data = (try decodeNext(reader, null)).? }},
    );
    if (true) return error.SkipZigTest;

    try testing.expectEqual(@as(?Instruction, null), try decodeNext(reader, null));
}

fn testExpectModInstr(op: Operation, dst: ModOperand, src: ModOperand, instr: ?Instruction) !void {
    try testing.expectEqual(op, instr.?.op);
    try testing.expectEqualDeep(dst, instr.?.payload.mod.getDst());
    try testing.expectEqualDeep(src, instr.?.payload.mod.getSrc());
}

fn testExpectModSpecialInstr(
    op: Operation,
    dst: ModOperand,
    src: ModSpecialInstr.SrcOperand,
    instr: ?Instruction,
) !void {
    try testing.expectEqual(op, instr.?.op);
    try testing.expectEqualDeep(ModSpecialInstr{ .dst = dst, .src = src }, instr.?.payload.mod_special);
}

fn testExpectDataInstr(op: Operation, reg: Register, imm: u16, instr: ?Instruction) !void {
    try testing.expectEqual(op, instr.?.op);
    try testing.expectEqualDeep(DataInstr{ .reg = reg, .imm = imm }, instr.?.payload.data);
}

fn testExpectAddrInstr(
    op: Operation,
    dst: AddrInstr.Operand,
    src: AddrInstr.Operand,
    instr: ?Instruction,
) !void {
    try testing.expectEqual(op, instr.?.op);
    try testing.expectEqualDeep(dst, instr.?.payload.addr.getDst());
    try testing.expectEqualDeep(src, instr.?.payload.addr.getSrc());
}
