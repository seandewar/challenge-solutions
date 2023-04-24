const std = @import("std");
const testing = std.testing;

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
    cmpsb,
    cmpsw,
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
    lodsb,
    lodsw,
    loop,
    loope,
    loopne,
    mov,
    movsb,
    movsw,
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
    ret,
    retf,
    rol,
    ror,
    sahf,
    sal,
    sar,
    sbb,
    scasb,
    scasw,
    shr,
    stc,
    std,
    sti,
    stosb,
    stosw,
    sub,
    @"test",
    wait,
    xchg,
    xlat,
    xor,

    rep_prefix,
    repne_prefix,
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
        0xa4 => .movsb,
        0xa5 => .movsw,
        0xa6 => .cmpsb,
        0xa7 => .cmpsw,
        0xaa => .stosb,
        0xab => .stosw,
        0xac => .lodsb,
        0xad => .lodsw,
        0xae => .scasb,
        0xaf => .scasw,
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
        0xf2 => .repne_prefix,
        0xf3 => .rep_prefix,
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
    const info: struct { w: bool, d: bool } = switch (first) {
        0x8d, 0xc4, 0xc5 => .{ .w = true, .d = true },
        else => .{ .w = first & 1 == 1, .d = (first >> 1) & 1 == 1 },
    };

    const second = try reader.readByte();
    const mod = @intCast(u2, (second >> 6) & 0b11);
    const rm = @intCast(u3, second & 0b111);
    const reg = switch (first) {
        0x8c, 0x8e => Register.getSegment(@intCast(u2, (second >> 3) & 0b11)),
        else => Register.getGeneralPurpose(@intCast(u3, (second >> 3) & 0b111), info.w),
    };
    const operand = try decodeModOperand(reader, mod, rm, info.w);

    return .{ .d = info.d, .reg = reg, .operand = operand };
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
    regs: Registers = .none,
    disp: u16 = 0,

    const Registers = enum {
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
            .{ .regs = @intToEnum(EffectiveAddr.Registers, rm) },
        1, 2 => .{ .regs = @intToEnum(EffectiveAddr.Registers, rm), .disp = disp },
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

fn testOpenListing(listing_name: []const u8) !std.fs.File {
    const listings_dir = std.fs.cwd().openDir("../computer_enhance/perfaware/part1", .{}) catch |err| switch (err) {
        error.FileNotFound => {
            std.log.err("Could not find the \"Computer, Enhance!\" listings directory! " ++
                "Make sure you clone submodules and run tests from the z8086 directory.", .{});
            return error.SkipZigTest;
        },
        else => return err,
    };

    var buf: [std.fs.MAX_PATH_BYTES - 1:0]u8 = undefined;
    return try listings_dir.openFileZ(try std.fmt.bufPrintZ(&buf, "listing_{s}", .{listing_name}), .{});
}

test "0037: single register mov" {
    var file = try testOpenListing("0037_single_register_mov");
    defer file.close();
    const reader = file.reader();

    try testExpectModInstr(.mov, .{ .reg = .cx }, .{ .reg = .bx }, try decodeNext(reader, null));
    try testing.expectEqual(@as(?Instruction, null), try decodeNext(reader, null));
}

test "0038: many register mov" {
    var file = try testOpenListing("0038_many_register_mov");
    defer file.close();
    const reader = file.reader();

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

test "0039: more movs" {
    var file = try testOpenListing("0039_more_movs");
    defer file.close();
    const reader = file.reader();

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
    try testExpectModInstr(.mov, .{ .reg = .al }, .{ .addr = .{ .regs = .bx_si } }, try decodeNext(reader, null));
    try testExpectModInstr(.mov, .{ .reg = .bx }, .{ .addr = .{ .regs = .bp_di } }, try decodeNext(reader, null));
    try testExpectModInstr(.mov, .{ .reg = .dx }, .{ .addr = .{ .regs = .bp } }, try decodeNext(reader, null));

    // Source address calculation plus 8-bit displacement.
    try testExpectModInstr(
        .mov,
        .{ .reg = .ah },
        .{ .addr = .{ .regs = .bx_si, .disp = 4 } },
        try decodeNext(reader, null),
    );

    // Source address calculation plus 16-bit displacement.
    try testExpectModInstr(
        .mov,
        .{ .reg = .al },
        .{ .addr = .{ .regs = .bx_si, .disp = 4999 } },
        try decodeNext(reader, null),
    );

    // Destination address calculation.
    try testExpectModInstr(.mov, .{ .addr = .{ .regs = .bx_di } }, .{ .reg = .cx }, try decodeNext(reader, null));
    try testExpectModInstr(.mov, .{ .addr = .{ .regs = .bp_si } }, .{ .reg = .cl }, try decodeNext(reader, null));
    try testExpectModInstr(.mov, .{ .addr = .{ .regs = .bp } }, .{ .reg = .ch }, try decodeNext(reader, null));

    try testing.expectEqual(@as(?Instruction, null), try decodeNext(reader, null));
}

test "0040: challenge movs" {
    var file = try testOpenListing("0040_challenge_movs");
    defer file.close();
    const reader = file.reader();

    // Signed displacements.
    try testExpectModInstr(
        .mov,
        .{ .reg = .ax },
        .{ .addr = .{ .regs = .bx_di, .disp = @bitCast(u8, @as(i8, -37)) } },
        try decodeNext(reader, null),
    );
    try testExpectModInstr(
        .mov,
        .{ .addr = .{ .regs = .si, .disp = @bitCast(u16, @as(i16, -300)) } },
        .{ .reg = .cx },
        try decodeNext(reader, null),
    );
    try testExpectModInstr(
        .mov,
        .{ .reg = .dx },
        .{ .addr = .{ .regs = .bx, .disp = @bitCast(u8, @as(i8, -32)) } },
        try decodeNext(reader, null),
    );

    // Explicit sizes.
    try testExpectModSpecialInstr(
        .mov,
        .{ .addr = .{ .regs = .bp_di } },
        .{ .imm_unsigned = 7 },
        try decodeNext(reader, null),
    );
    try testExpectModSpecialInstr(
        .mov,
        .{ .addr = .{ .regs = .di, .disp = 901 } },
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

test "0041: add sub cmp jnz" {
    var file = try testOpenListing("0041_add_sub_cmp_jnz");
    defer file.close();
    const reader = file.reader();

    for ([_]Operation{ .add, .sub, .cmp }) |op| {
        try testExpectModInstr(op, .{ .reg = .bx }, .{ .addr = .{ .regs = .bx_si } }, try decodeNext(reader, null));
        try testExpectModInstr(op, .{ .reg = .bx }, .{ .addr = .{ .regs = .bp } }, try decodeNext(reader, null));
        try testExpectModSpecialInstr(op, .{ .reg = .si }, .{ .imm_signed = 2 }, try decodeNext(reader, null));
        try testExpectModSpecialInstr(op, .{ .reg = .bp }, .{ .imm_signed = 2 }, try decodeNext(reader, null));
        try testExpectModSpecialInstr(op, .{ .reg = .cx }, .{ .imm_signed = 8 }, try decodeNext(reader, null));
        try testExpectModInstr(op, .{ .reg = .bx }, .{ .addr = .{ .regs = .bp } }, try decodeNext(reader, null));
        try testExpectModInstr(op, .{ .reg = .cx }, .{ .addr = .{ .regs = .bx, .disp = 2 } }, try decodeNext(reader, null));
        try testExpectModInstr(op, .{ .reg = .bh }, .{ .addr = .{ .regs = .bp_si, .disp = 4 } }, try decodeNext(reader, null));
        try testExpectModInstr(op, .{ .reg = .di }, .{ .addr = .{ .regs = .bp_di, .disp = 6 } }, try decodeNext(reader, null));
        try testExpectModInstr(op, .{ .addr = .{ .regs = .bx_si } }, .{ .reg = .bx }, try decodeNext(reader, null));
        try testExpectModInstr(op, .{ .addr = .{ .regs = .bp } }, .{ .reg = .bx }, try decodeNext(reader, null));
        try testExpectModInstr(op, .{ .addr = .{ .regs = .bp } }, .{ .reg = .bx }, try decodeNext(reader, null));
        try testExpectModInstr(op, .{ .addr = .{ .regs = .bx, .disp = 2 } }, .{ .reg = .cx }, try decodeNext(reader, null));
        try testExpectModInstr(op, .{ .addr = .{ .regs = .bp_si, .disp = 4 } }, .{ .reg = .bh }, try decodeNext(reader, null));
        try testExpectModInstr(op, .{ .addr = .{ .regs = .bp_di, .disp = 6 } }, .{ .reg = .di }, try decodeNext(reader, null));
        try testExpectModSpecialInstr(op, .{ .addr = .{ .regs = .bx } }, .{ .imm_unsigned = 34 }, try decodeNext(reader, null));

        {
            const addr: EffectiveAddr = switch (op) {
                .add => .{ .regs = .bp_si, .disp = 1000 },
                .sub => .{ .regs = .bx_di },
                .cmp => .{ .disp = 4834 },
                else => unreachable,
            };
            try testExpectModSpecialInstr(op, .{ .addr = addr }, .{ .imm_signed = 29 }, try decodeNext(reader, null));
        }

        try testExpectModInstr(op, .{ .reg = .ax }, .{ .addr = .{ .regs = .bp } }, try decodeNext(reader, null));
        try testExpectModInstr(op, .{ .reg = .al }, .{ .addr = .{ .regs = .bx_si } }, try decodeNext(reader, null));
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

test "0042: completionist decode" {
    var file = try testOpenListing("0042_completionist_decode");
    defer file.close();
    const reader = file.reader();

    try testExpectModInstr(.mov, .{ .reg = .si }, .{ .reg = .bx }, try decodeNext(reader, null));
    try testExpectModInstr(.mov, .{ .reg = .dh }, .{ .reg = .al }, try decodeNext(reader, null));
    try testExpectDataInstr(.mov, .cl, 12, try decodeNext(reader, null));
    try testExpectDataInstr(.mov, .ch, @bitCast(u8, @as(i8, -12)), try decodeNext(reader, null));
    try testExpectDataInstr(.mov, .cx, 12, try decodeNext(reader, null));
    try testExpectDataInstr(.mov, .cx, @bitCast(u16, @as(i16, -12)), try decodeNext(reader, null));
    try testExpectDataInstr(.mov, .dx, 3948, try decodeNext(reader, null));
    try testExpectDataInstr(.mov, .dx, @bitCast(u16, @as(i16, -3948)), try decodeNext(reader, null));
    try testExpectModInstr(.mov, .{ .reg = .al }, .{ .addr = .{ .regs = .bx_si } }, try decodeNext(reader, null));
    try testExpectModInstr(.mov, .{ .reg = .bx }, .{ .addr = .{ .regs = .bp_di } }, try decodeNext(reader, null));
    try testExpectModInstr(.mov, .{ .reg = .dx }, .{ .addr = .{ .regs = .bp } }, try decodeNext(reader, null));
    try testExpectModInstr(.mov, .{ .reg = .ah }, .{ .addr = .{ .regs = .bx_si, .disp = 4 } }, try decodeNext(reader, null));
    try testExpectModInstr(.mov, .{ .reg = .al }, .{ .addr = .{ .regs = .bx_si, .disp = 4999 } }, try decodeNext(reader, null));
    try testExpectModInstr(.mov, .{ .addr = .{ .regs = .bx_di } }, .{ .reg = .cx }, try decodeNext(reader, null));
    try testExpectModInstr(.mov, .{ .addr = .{ .regs = .bp_si } }, .{ .reg = .cl }, try decodeNext(reader, null));
    try testExpectModInstr(.mov, .{ .addr = .{ .regs = .bp } }, .{ .reg = .ch }, try decodeNext(reader, null));
    try testExpectModInstr(
        .mov,
        .{ .reg = .ax },
        .{ .addr = .{ .regs = .bx_di, .disp = @bitCast(u8, @as(i8, -37)) } },
        try decodeNext(reader, null),
    );
    try testExpectModInstr(
        .mov,
        .{ .addr = .{ .regs = .si, .disp = @bitCast(u16, @as(i16, -300)) } },
        .{ .reg = .cx },
        try decodeNext(reader, null),
    );
    try testExpectModInstr(
        .mov,
        .{ .reg = .dx },
        .{ .addr = .{ .regs = .bx, .disp = @bitCast(u8, @as(i8, -32)) } },
        try decodeNext(reader, null),
    );
    try testExpectModSpecialInstr(
        .mov,
        .{ .addr = .{ .regs = .bp_di } },
        .{ .imm_unsigned = 7 },
        try decodeNext(reader, null),
    );
    try testExpectModSpecialInstr(
        .mov,
        .{ .addr = .{ .regs = .di, .disp = 901 } },
        .{ .imm_unsigned = 347 },
        try decodeNext(reader, null),
    );
    try testExpectModInstr(.mov, .{ .reg = .bp }, .{ .addr = .{ .disp = 5 } }, try decodeNext(reader, null));
    try testExpectModInstr(.mov, .{ .reg = .bx }, .{ .addr = .{ .disp = 3458 } }, try decodeNext(reader, null));
    try testExpectAddrInstr(.mov, .{ .reg = .ax }, .{ .addr = 2555 }, try decodeNext(reader, null));
    try testExpectAddrInstr(.mov, .{ .reg = .ax }, .{ .addr = 16 }, try decodeNext(reader, null));
    try testExpectAddrInstr(.mov, .{ .addr = 2554 }, .{ .reg = .ax }, try decodeNext(reader, null));
    try testExpectAddrInstr(.mov, .{ .addr = 15 }, .{ .reg = .ax }, try decodeNext(reader, null));

    try testExpectModSpecialInstr(.push, .{ .addr = .{ .regs = .bp_si } }, .none, try decodeNext(reader, null));
    try testExpectModSpecialInstr(.push, .{ .addr = .{ .disp = 3000 } }, .none, try decodeNext(reader, null));
    try testExpectModSpecialInstr(
        .push,
        .{ .addr = .{ .regs = .bx_di, .disp = @bitCast(u8, @as(i8, -30)) } },
        .none,
        try decodeNext(reader, null),
    );
    try testing.expectEqual(Instruction{ .op = .push, .payload = .{ .reg = .cx } }, (try decodeNext(reader, null)).?);
    try testing.expectEqual(Instruction{ .op = .push, .payload = .{ .reg = .ax } }, (try decodeNext(reader, null)).?);
    try testing.expectEqual(Instruction{ .op = .push, .payload = .{ .reg = .dx } }, (try decodeNext(reader, null)).?);
    try testing.expectEqual(Instruction{ .op = .push, .payload = .{ .reg = .cs } }, (try decodeNext(reader, null)).?);

    try testExpectModSpecialInstr(.pop, .{ .addr = .{ .regs = .bp_si } }, .none, try decodeNext(reader, null));
    try testExpectModSpecialInstr(.pop, .{ .addr = .{ .disp = 3 } }, .none, try decodeNext(reader, null));
    try testExpectModSpecialInstr(
        .pop,
        .{ .addr = .{ .regs = .bx_di, .disp = @bitCast(u16, @as(i16, -3000)) } },
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
        .{ .addr = .{ .regs = .bp, .disp = @bitCast(u16, @as(i16, -1000)) } },
        try decodeNext(reader, null),
    );
    try testExpectModInstr(.xchg, .{ .reg = .bp }, .{ .addr = .{ .regs = .bx, .disp = 50 } }, try decodeNext(reader, null));

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

    try testExpectDataInstr(.in, .al, 200, try decodeNext(reader, null));
    try testing.expectEqual(
        Instruction{ .op = .in, .payload = .{ .regs = .{ .dst = .al, .src = .dx } } },
        (try decodeNext(reader, null)).?,
    );
    try testing.expectEqual(
        Instruction{ .op = .in, .payload = .{ .regs = .{ .dst = .ax, .src = .dx } } },
        (try decodeNext(reader, null)).?,
    );

    try testExpectDataInstr(.out, .ax, 44, try decodeNext(reader, null));
    try testing.expectEqual(
        Instruction{ .op = .out, .payload = .{ .regs = .{ .dst = .al, .src = .dx } } },
        (try decodeNext(reader, null)).?,
    );

    try testing.expectEqual(Instruction{ .op = .xlat, .payload = .none }, (try decodeNext(reader, null)).?);

    for ([_]Operation{ .lea, .lds, .les }) |op| {
        try testExpectModInstr(
            op,
            .{ .reg = .ax },
            .{ .addr = .{ .regs = .bx_di, .disp = 1420 } },
            try decodeNext(reader, null),
        );
        try testExpectModInstr(
            op,
            .{ .reg = .bx },
            .{ .addr = .{ .regs = .bp, .disp = @bitCast(u8, @as(i8, -50)) } },
            try decodeNext(reader, null),
        );
        try testExpectModInstr(
            op,
            .{ .reg = .sp },
            .{ .addr = .{ .regs = .bp, .disp = @bitCast(u16, @as(i16, -1003)) } },
            try decodeNext(reader, null),
        );
        try testExpectModInstr(
            op,
            .{ .reg = .di },
            .{ .addr = .{ .regs = .bx_si, .disp = @bitCast(u8, @as(i8, -7)) } },
            try decodeNext(reader, null),
        );
    }

    try testing.expectEqual(Instruction{ .op = .lahf, .payload = .none }, (try decodeNext(reader, null)).?);
    try testing.expectEqual(Instruction{ .op = .sahf, .payload = .none }, (try decodeNext(reader, null)).?);
    try testing.expectEqual(Instruction{ .op = .pushf, .payload = .none }, (try decodeNext(reader, null)).?);
    try testing.expectEqual(Instruction{ .op = .popf, .payload = .none }, (try decodeNext(reader, null)).?);

    const testAddSub = struct {
        fn f(r: anytype, op: Operation) !void {
            try testExpectModInstr(op, .{ .reg = .cx }, .{ .addr = .{ .regs = .bp } }, try decodeNext(r, null));
            try testExpectModInstr(op, .{ .reg = .dx }, .{ .addr = .{ .regs = .bx_si } }, try decodeNext(r, null));
            try testExpectModInstr(op, .{ .addr = .{ .regs = .bp_di, .disp = 5000 } }, .{ .reg = .ah }, try decodeNext(r, null));
            try testExpectModInstr(op, .{ .addr = .{ .regs = .bx } }, .{ .reg = .al }, try decodeNext(r, null));
            try testExpectModSpecialInstr(op, .{ .reg = .sp }, .{ .imm_unsigned = 392 }, try decodeNext(r, null));
            try testExpectModSpecialInstr(op, .{ .reg = .si }, .{ .imm_signed = 5 }, try decodeNext(r, null));
            try testExpectDataInstr(op, .ax, 1000, try decodeNext(r, null));
            try testExpectModSpecialInstr(op, .{ .reg = .ah }, .{ .imm_unsigned = 30 }, try decodeNext(r, null));
            try testExpectDataInstr(op, .al, 9, try decodeNext(r, null));
            try testExpectModInstr(op, .{ .reg = .cx }, .{ .reg = .bx }, try decodeNext(r, null));
            try testExpectModInstr(op, .{ .reg = .ch }, .{ .reg = .al }, try decodeNext(r, null));
        }
    }.f;

    try testAddSub(reader, .add);
    try testAddSub(reader, .adc);

    const testIncDecNeg = struct {
        fn f(r: anytype, op: Operation) !void {
            if (op == .neg) {
                try testExpectModSpecialInstr(op, .{ .reg = .ax }, .none, try decodeNext(r, null));
                try testExpectModSpecialInstr(op, .{ .reg = .cx }, .none, try decodeNext(r, null));
            } else {
                try testing.expectEqual(Instruction{ .op = op, .payload = .{ .reg = .ax } }, (try decodeNext(r, null)).?);
                try testing.expectEqual(Instruction{ .op = op, .payload = .{ .reg = .cx } }, (try decodeNext(r, null)).?);
            }

            try testExpectModSpecialInstr(op, .{ .reg = .dh }, .none, try decodeNext(r, null));
            try testExpectModSpecialInstr(op, .{ .reg = .al }, .none, try decodeNext(r, null));
            try testExpectModSpecialInstr(op, .{ .reg = .ah }, .none, try decodeNext(r, null));

            if (op == .neg) {
                try testExpectModSpecialInstr(op, .{ .reg = .sp }, .none, try decodeNext(r, null));
                try testExpectModSpecialInstr(op, .{ .reg = .di }, .none, try decodeNext(r, null));
            } else {
                try testing.expectEqual(Instruction{ .op = op, .payload = .{ .reg = .sp } }, (try decodeNext(r, null)).?);
                try testing.expectEqual(Instruction{ .op = op, .payload = .{ .reg = .di } }, (try decodeNext(r, null)).?);
            }

            try testExpectModSpecialInstr(op, .{ .addr = .{ .regs = .bp, .disp = 1002 } }, .none, try decodeNext(r, null));
            try testExpectModSpecialInstr(op, .{ .addr = .{ .regs = .bx, .disp = 39 } }, .none, try decodeNext(r, null));
            try testExpectModSpecialInstr(op, .{ .addr = .{ .regs = .bx_si, .disp = 5 } }, .none, try decodeNext(r, null));
            try testExpectModSpecialInstr(
                op,
                .{ .addr = .{ .regs = .bp_di, .disp = @bitCast(u16, @as(i16, -10044)) } },
                .none,
                try decodeNext(r, null),
            );
            try testExpectModSpecialInstr(op, .{ .addr = .{ .disp = 9349 } }, .none, try decodeNext(r, null));
            try testExpectModSpecialInstr(op, .{ .addr = .{ .regs = .bp } }, .none, try decodeNext(r, null));
        }
    }.f;

    try testIncDecNeg(reader, .inc);

    try testing.expectEqual(Instruction{ .op = .aaa, .payload = .none }, (try decodeNext(reader, null)).?);
    try testing.expectEqual(Instruction{ .op = .daa, .payload = .none }, (try decodeNext(reader, null)).?);

    try testAddSub(reader, .sub);
    try testAddSub(reader, .sbb);

    try testIncDecNeg(reader, .dec);
    try testIncDecNeg(reader, .neg);

    const testCmpTest = struct {
        fn f(r: anytype, op: Operation) !void {
            try testExpectModInstr(op, .{ .reg = .bx }, .{ .reg = .cx }, try decodeNext(r, null));

            if (op == .cmp) {
                try testExpectModInstr(op, .{ .reg = .dh }, .{ .addr = .{ .regs = .bp, .disp = 390 } }, try decodeNext(r, null));
            } else {
                try testExpectModInstr(op, .{ .addr = .{ .regs = .bp, .disp = 390 } }, .{ .reg = .dh }, try decodeNext(r, null));
            }

            try testExpectModInstr(op, .{ .addr = .{ .regs = .bp, .disp = 2 } }, .{ .reg = .si }, try decodeNext(r, null));
            try testExpectModSpecialInstr(op, .{ .reg = .bl }, .{ .imm_unsigned = 20 }, try decodeNext(r, null));
            try testExpectModSpecialInstr(op, .{ .addr = .{ .regs = .bx } }, .{ .imm_unsigned = 34 }, try decodeNext(r, null));
            try testExpectDataInstr(op, .ax, 23909, try decodeNext(r, null));
        }
    }.f;

    try testCmpTest(reader, .cmp);

    try testing.expectEqual(Instruction{ .op = .aas, .payload = .none }, (try decodeNext(reader, null)).?);
    try testing.expectEqual(Instruction{ .op = .das, .payload = .none }, (try decodeNext(reader, null)).?);

    try testExpectModSpecialInstr(.mul, .{ .reg = .al }, .none, try decodeNext(reader, null));
    try testExpectModSpecialInstr(.mul, .{ .reg = .cx }, .none, try decodeNext(reader, null));
    try testExpectModSpecialInstr(.mul, .{ .addr = .{ .regs = .bp } }, .none, try decodeNext(reader, null));
    try testExpectModSpecialInstr(.mul, .{ .addr = .{ .regs = .bx_di, .disp = 500 } }, .none, try decodeNext(reader, null));

    try testExpectModSpecialInstr(.imul, .{ .reg = .ch }, .none, try decodeNext(reader, null));
    try testExpectModSpecialInstr(.imul, .{ .reg = .dx }, .none, try decodeNext(reader, null));
    try testExpectModSpecialInstr(.imul, .{ .addr = .{ .regs = .bx } }, .none, try decodeNext(reader, null));
    try testExpectModSpecialInstr(.imul, .{ .addr = .{ .disp = 9483 } }, .none, try decodeNext(reader, null));

    try testing.expectEqual(Instruction{ .op = .aam, .payload = .none }, (try decodeNext(reader, null)).?);

    try testExpectModSpecialInstr(.div, .{ .reg = .bl }, .none, try decodeNext(reader, null));
    try testExpectModSpecialInstr(.div, .{ .reg = .sp }, .none, try decodeNext(reader, null));
    try testExpectModSpecialInstr(.div, .{ .addr = .{ .regs = .bx_si, .disp = 2990 } }, .none, try decodeNext(reader, null));
    try testExpectModSpecialInstr(.div, .{ .addr = .{ .regs = .bp_di, .disp = 1000 } }, .none, try decodeNext(reader, null));

    try testExpectModSpecialInstr(.idiv, .{ .reg = .ax }, .none, try decodeNext(reader, null));
    try testExpectModSpecialInstr(.idiv, .{ .reg = .si }, .none, try decodeNext(reader, null));
    try testExpectModSpecialInstr(.idiv, .{ .addr = .{ .regs = .bp_si } }, .none, try decodeNext(reader, null));
    try testExpectModSpecialInstr(.idiv, .{ .addr = .{ .regs = .bx, .disp = 493 } }, .none, try decodeNext(reader, null));

    try testing.expectEqual(Instruction{ .op = .aad, .payload = .none }, (try decodeNext(reader, null)).?);
    try testing.expectEqual(Instruction{ .op = .cbw, .payload = .none }, (try decodeNext(reader, null)).?);
    try testing.expectEqual(Instruction{ .op = .cwd, .payload = .none }, (try decodeNext(reader, null)).?);

    try testExpectModSpecialInstr(.not, .{ .reg = .ah }, .none, try decodeNext(reader, null));
    try testExpectModSpecialInstr(.not, .{ .reg = .bl }, .none, try decodeNext(reader, null));
    try testExpectModSpecialInstr(.not, .{ .reg = .sp }, .none, try decodeNext(reader, null));
    try testExpectModSpecialInstr(.not, .{ .reg = .si }, .none, try decodeNext(reader, null));
    try testExpectModSpecialInstr(.not, .{ .addr = .{ .regs = .bp } }, .none, try decodeNext(reader, null));
    try testExpectModSpecialInstr(.not, .{ .addr = .{ .regs = .bp, .disp = 9905 } }, .none, try decodeNext(reader, null));

    for ([_]ModSpecialInstr.SrcOperand{ .{ .imm_unsigned = 1 }, .cl }) |src| {
        try testExpectModSpecialInstr(.sal, .{ .reg = .ah }, src, try decodeNext(reader, null));
        try testExpectModSpecialInstr(.shr, .{ .reg = .ax }, src, try decodeNext(reader, null));
        try testExpectModSpecialInstr(.sar, .{ .reg = .bx }, src, try decodeNext(reader, null));
        try testExpectModSpecialInstr(.rol, .{ .reg = .cx }, src, try decodeNext(reader, null));
        try testExpectModSpecialInstr(.ror, .{ .reg = .dh }, src, try decodeNext(reader, null));
        try testExpectModSpecialInstr(.rcl, .{ .reg = .sp }, src, try decodeNext(reader, null));
        try testExpectModSpecialInstr(.rcr, .{ .reg = .bp }, src, try decodeNext(reader, null));

        try testExpectModSpecialInstr(.sal, .{ .addr = .{ .regs = .bp, .disp = 5 } }, src, try decodeNext(reader, null));
        try testExpectModSpecialInstr(
            .shr,
            .{ .addr = .{ .regs = .bx_si, .disp = @bitCast(u16, @as(i16, -199)) } },
            src,
            try decodeNext(reader, null),
        );
        try testExpectModSpecialInstr(
            .sar,
            .{ .addr = .{ .regs = .bx_di, .disp = @bitCast(u16, @as(i16, -300)) } },
            src,
            try decodeNext(reader, null),
        );
        try testExpectModSpecialInstr(.rol, .{ .addr = .{ .regs = .bp } }, src, try decodeNext(reader, null));
        try testExpectModSpecialInstr(.ror, .{ .addr = .{ .disp = 4938 } }, src, try decodeNext(reader, null));
        try testExpectModSpecialInstr(.rcl, .{ .addr = .{ .disp = 3 } }, src, try decodeNext(reader, null));
        try testExpectModSpecialInstr(.rcr, .{ .addr = .{ .regs = .bx } }, src, try decodeNext(reader, null));
    }

    const testAndOrXor = struct {
        fn f(r: anytype, op: Operation) !void {
            try testExpectModInstr(op, .{ .reg = .al }, .{ .reg = .ah }, try decodeNext(r, null));
            try testExpectModInstr(op, .{ .reg = .ch }, .{ .reg = .cl }, try decodeNext(r, null));
            try testExpectModInstr(op, .{ .reg = .bp }, .{ .reg = .si }, try decodeNext(r, null));
            try testExpectModInstr(op, .{ .reg = .di }, .{ .reg = .sp }, try decodeNext(r, null));
            try testExpectDataInstr(op, .al, 93, try decodeNext(r, null));
            try testExpectDataInstr(op, .ax, 20392, try decodeNext(r, null));
            try testExpectModInstr(op, .{ .addr = .{ .regs = .bp_si, .disp = 10 } }, .{ .reg = .ch }, try decodeNext(r, null));
            try testExpectModInstr(op, .{ .addr = .{ .regs = .bx_di, .disp = 1000 } }, .{ .reg = .dx }, try decodeNext(r, null));
            try testExpectModInstr(op, .{ .reg = .bx }, .{ .addr = .{ .regs = .bp } }, try decodeNext(r, null));
            try testExpectModInstr(op, .{ .reg = .cx }, .{ .addr = .{ .disp = 4384 } }, try decodeNext(r, null));
            try testExpectModSpecialInstr(
                op,
                .{ .addr = .{ .regs = .bp, .disp = @bitCast(u8, @as(i8, -39)) } },
                .{ .imm_unsigned = 239 },
                try decodeNext(r, null),
            );
            try testExpectModSpecialInstr(
                op,
                .{ .addr = .{ .regs = .bx_si, .disp = @bitCast(u16, @as(i16, -4332)) } },
                .{ .imm_unsigned = 10328 },
                try decodeNext(r, null),
            );
        }
    }.f;

    try testAndOrXor(reader, .@"and");

    try testCmpTest(reader, .@"test");

    try testAndOrXor(reader, .@"or");
    try testAndOrXor(reader, .xor);

    try testing.expectEqual(Instruction{ .op = .rep_prefix, .payload = .none }, (try decodeNext(reader, null)).?);
    try testing.expectEqual(Instruction{ .op = .movsb, .payload = .none }, (try decodeNext(reader, null)).?);
    try testing.expectEqual(Instruction{ .op = .rep_prefix, .payload = .none }, (try decodeNext(reader, null)).?);
    try testing.expectEqual(Instruction{ .op = .cmpsb, .payload = .none }, (try decodeNext(reader, null)).?);
    try testing.expectEqual(Instruction{ .op = .rep_prefix, .payload = .none }, (try decodeNext(reader, null)).?);
    try testing.expectEqual(Instruction{ .op = .scasb, .payload = .none }, (try decodeNext(reader, null)).?);
    try testing.expectEqual(Instruction{ .op = .rep_prefix, .payload = .none }, (try decodeNext(reader, null)).?);
    try testing.expectEqual(Instruction{ .op = .lodsb, .payload = .none }, (try decodeNext(reader, null)).?);
    try testing.expectEqual(Instruction{ .op = .rep_prefix, .payload = .none }, (try decodeNext(reader, null)).?);
    try testing.expectEqual(Instruction{ .op = .movsw, .payload = .none }, (try decodeNext(reader, null)).?);
    try testing.expectEqual(Instruction{ .op = .rep_prefix, .payload = .none }, (try decodeNext(reader, null)).?);
    try testing.expectEqual(Instruction{ .op = .cmpsw, .payload = .none }, (try decodeNext(reader, null)).?);
    try testing.expectEqual(Instruction{ .op = .rep_prefix, .payload = .none }, (try decodeNext(reader, null)).?);
    try testing.expectEqual(Instruction{ .op = .scasw, .payload = .none }, (try decodeNext(reader, null)).?);
    try testing.expectEqual(Instruction{ .op = .rep_prefix, .payload = .none }, (try decodeNext(reader, null)).?);
    try testing.expectEqual(Instruction{ .op = .lodsw, .payload = .none }, (try decodeNext(reader, null)).?);

    try testing.expectEqual(Instruction{ .op = .rep_prefix, .payload = .none }, (try decodeNext(reader, null)).?);
    try testing.expectEqual(Instruction{ .op = .stosb, .payload = .none }, (try decodeNext(reader, null)).?);
    try testing.expectEqual(Instruction{ .op = .rep_prefix, .payload = .none }, (try decodeNext(reader, null)).?);
    try testing.expectEqual(Instruction{ .op = .stosw, .payload = .none }, (try decodeNext(reader, null)).?);

    try testExpectModSpecialInstr(.call, .{ .addr = .{ .disp = 39201 } }, .none, try decodeNext(reader, null));
    try testExpectModSpecialInstr(
        .call,
        .{ .addr = .{ .regs = .bp, .disp = @bitCast(u8, @as(i8, -100)) } },
        .none,
        try decodeNext(reader, null),
    );
    try testExpectModSpecialInstr(.call, .{ .reg = .sp }, .none, try decodeNext(reader, null));
    try testExpectModSpecialInstr(.call, .{ .reg = .ax }, .none, try decodeNext(reader, null));

    try testExpectModSpecialInstr(.jmp, .{ .reg = .ax }, .none, try decodeNext(reader, null));
    try testExpectModSpecialInstr(.jmp, .{ .reg = .di }, .none, try decodeNext(reader, null));
    try testExpectModSpecialInstr(.jmp, .{ .addr = .{ .disp = 12 } }, .none, try decodeNext(reader, null));
    try testExpectModSpecialInstr(.jmp, .{ .addr = .{ .disp = 4395 } }, .none, try decodeNext(reader, null));

    try testing.expectEqual(
        Instruction{ .op = .ret, .payload = .{ .imm = @bitCast(u16, @as(i16, -7)) } },
        (try decodeNext(reader, null)).?,
    );
    try testing.expectEqual(Instruction{ .op = .ret, .payload = .{ .imm = 500 } }, (try decodeNext(reader, null)).?);
    try testing.expectEqual(Instruction{ .op = .ret, .payload = .none }, (try decodeNext(reader, null)).?);

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

    try testing.expectEqual(Instruction{ .op = .int, .payload = .{ .imm = 13 } }, (try decodeNext(reader, null)).?);
    try testing.expectEqual(Instruction{ .op = .int, .payload = .{ .imm = 3 } }, (try decodeNext(reader, null)).?);

    try testing.expectEqual(Instruction{ .op = .into, .payload = .none }, (try decodeNext(reader, null)).?);
    try testing.expectEqual(Instruction{ .op = .iret, .payload = .none }, (try decodeNext(reader, null)).?);

    try testing.expectEqual(Instruction{ .op = .clc, .payload = .none }, (try decodeNext(reader, null)).?);
    try testing.expectEqual(Instruction{ .op = .cmc, .payload = .none }, (try decodeNext(reader, null)).?);
    try testing.expectEqual(Instruction{ .op = .stc, .payload = .none }, (try decodeNext(reader, null)).?);
    try testing.expectEqual(Instruction{ .op = .cld, .payload = .none }, (try decodeNext(reader, null)).?);
    try testing.expectEqual(Instruction{ .op = .std, .payload = .none }, (try decodeNext(reader, null)).?);
    try testing.expectEqual(Instruction{ .op = .cli, .payload = .none }, (try decodeNext(reader, null)).?);
    try testing.expectEqual(Instruction{ .op = .sti, .payload = .none }, (try decodeNext(reader, null)).?);
    try testing.expectEqual(Instruction{ .op = .hlt, .payload = .none }, (try decodeNext(reader, null)).?);
    try testing.expectEqual(Instruction{ .op = .wait, .payload = .none }, (try decodeNext(reader, null)).?);

    try testing.expectEqual(Instruction{ .op = .lock_prefix, .payload = .none }, (try decodeNext(reader, null)).?);
    try testExpectModSpecialInstr(.not, .{ .addr = .{ .regs = .bp, .disp = 9905 } }, .none, try decodeNext(reader, null));
    try testing.expectEqual(Instruction{ .op = .lock_prefix, .payload = .none }, (try decodeNext(reader, null)).?);
    try testExpectModInstr(.xchg, .{ .reg = .al }, .{ .addr = .{ .disp = 100 } }, try decodeNext(reader, null));

    try testing.expectEqual(Instruction{ .op = .cs_prefix, .payload = .none }, (try decodeNext(reader, null)).?);
    try testExpectModInstr(.mov, .{ .reg = .al }, .{ .addr = .{ .regs = .bx_si } }, try decodeNext(reader, null));
    try testing.expectEqual(Instruction{ .op = .ds_prefix, .payload = .none }, (try decodeNext(reader, null)).?);
    try testExpectModInstr(.mov, .{ .reg = .bx }, .{ .addr = .{ .regs = .bp_di } }, try decodeNext(reader, null));
    try testing.expectEqual(Instruction{ .op = .es_prefix, .payload = .none }, (try decodeNext(reader, null)).?);
    try testExpectModInstr(.mov, .{ .reg = .dx }, .{ .addr = .{ .regs = .bp } }, try decodeNext(reader, null));
    try testing.expectEqual(Instruction{ .op = .ss_prefix, .payload = .none }, (try decodeNext(reader, null)).?);
    try testExpectModInstr(.mov, .{ .reg = .ah }, .{ .addr = .{ .regs = .bx_si, .disp = 4 } }, try decodeNext(reader, null));

    try testing.expectEqual(Instruction{ .op = .ss_prefix, .payload = .none }, (try decodeNext(reader, null)).?);
    try testExpectModInstr(
        .@"and",
        .{ .addr = .{ .regs = .bp_si, .disp = 10 } },
        .{ .reg = .ch },
        try decodeNext(reader, null),
    );
    try testing.expectEqual(Instruction{ .op = .ds_prefix, .payload = .none }, (try decodeNext(reader, null)).?);
    try testExpectModInstr(
        .@"or",
        .{ .addr = .{ .regs = .bx_di, .disp = 1000 } },
        .{ .reg = .dx },
        try decodeNext(reader, null),
    );
    try testing.expectEqual(Instruction{ .op = .es_prefix, .payload = .none }, (try decodeNext(reader, null)).?);
    try testExpectModInstr(.xor, .{ .reg = .bx }, .{ .addr = .{ .regs = .bp } }, try decodeNext(reader, null));
    try testing.expectEqual(Instruction{ .op = .es_prefix, .payload = .none }, (try decodeNext(reader, null)).?);
    try testExpectModInstr(.cmp, .{ .reg = .cx }, .{ .addr = .{ .disp = 4384 } }, try decodeNext(reader, null));
    try testing.expectEqual(Instruction{ .op = .cs_prefix, .payload = .none }, (try decodeNext(reader, null)).?);
    try testExpectModSpecialInstr(
        .@"test",
        .{ .addr = .{ .regs = .bp, .disp = @bitCast(u8, @as(i8, -39)) } },
        .{ .imm_unsigned = 239 },
        try decodeNext(reader, null),
    );
    try testing.expectEqual(Instruction{ .op = .cs_prefix, .payload = .none }, (try decodeNext(reader, null)).?);
    try testExpectModSpecialInstr(
        .sbb,
        .{ .addr = .{ .regs = .bx_si, .disp = @bitCast(u16, @as(i16, -4332)) } },
        .{ .imm_unsigned = 10328 },
        try decodeNext(reader, null),
    );

    try testing.expectEqual(Instruction{ .op = .lock_prefix, .payload = .none }, (try decodeNext(reader, null)).?);
    try testing.expectEqual(Instruction{ .op = .cs_prefix, .payload = .none }, (try decodeNext(reader, null)).?);
    try testExpectModSpecialInstr(.not, .{ .addr = .{ .regs = .bp, .disp = 9905 } }, .none, try decodeNext(reader, null));

    try testing.expectEqual(
        Instruction{ .op = .call, .payload = .{ .interseg_addr = .{ .cs = 123, .addr = 456 } } },
        (try decodeNext(reader, null)).?,
    );
    try testing.expectEqual(
        Instruction{ .op = .jmp, .payload = .{ .interseg_addr = .{ .cs = 789, .addr = 34 } } },
        (try decodeNext(reader, null)).?,
    );

    try testExpectModInstr(
        .mov,
        .{ .addr = .{ .regs = .bx_si, .disp = 59 } },
        .{ .reg = .es },
        try decodeNext(reader, null),
    );

    try testing.expectEqual(Instruction{ .op = .jmp, .payload = .{ .ip_disp = 1753 } }, (try decodeNext(reader, null)).?);
    try testing.expectEqual(Instruction{ .op = .call, .payload = .{ .ip_disp = 10934 } }, (try decodeNext(reader, null)).?);

    try testing.expectEqual(Instruction{ .op = .retf, .payload = .{ .imm = 17556 } }, (try decodeNext(reader, null)).?);
    try testing.expectEqual(Instruction{ .op = .ret, .payload = .{ .imm = 17560 } }, (try decodeNext(reader, null)).?);
    try testing.expectEqual(Instruction{ .op = .retf, .payload = .none }, (try decodeNext(reader, null)).?);
    try testing.expectEqual(Instruction{ .op = .ret, .payload = .none }, (try decodeNext(reader, null)).?);

    for (0..2) |_| try testExpectModSpecialInstr(
        .call,
        .{ .addr = .{ .regs = .bp_si, .disp = @bitCast(u8, @as(i8, -0x3a)) } },
        .none,
        try decodeNext(reader, null),
    );
    for (0..2) |_| try testExpectModSpecialInstr(.jmp, .{ .addr = .{ .regs = .di } }, .none, try decodeNext(reader, null));

    try testing.expectEqual(
        Instruction{ .op = .jmp, .payload = .{ .interseg_addr = .{ .cs = 21862, .addr = 30600 } } },
        (try decodeNext(reader, null)).?,
    );

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

fn testExpectAddrInstr(op: Operation, dst: AddrInstr.Operand, src: AddrInstr.Operand, instr: ?Instruction) !void {
    try testing.expectEqual(op, instr.?.op);
    try testing.expectEqualDeep(dst, instr.?.payload.addr.getDst());
    try testing.expectEqualDeep(src, instr.?.payload.addr.getSrc());
}
