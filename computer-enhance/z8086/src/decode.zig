const std = @import("std");
const testing = std.testing;

const decode = @This();

pub fn InstrIterator(comptime InnerReader: type, comptime prev_bytes_cap: usize) type {
    return struct {
        pub const prev_bytes_capacity = prev_bytes_cap;
        prev_bytes: std.BoundedArray(u8, prev_bytes_cap) = .{},
        inner_reader: InnerReader,

        const ReadError = InnerReader.Error || error{Overflow};
        const Reader = std.io.Reader(*@This(), ReadError, read);

        pub inline fn next(self: *@This()) !?Instr {
            self.prev_bytes.len = 0;
            return decode.nextInstr(Reader{ .context = self });
        }

        pub inline fn getPrevBytes(self: @This()) []const u8 {
            return self.prev_bytes.slice();
        }

        fn read(self: *@This(), buf: []u8) ReadError!usize {
            const slice = buf[0..try self.inner_reader.read(buf)];
            try self.prev_bytes.appendSlice(slice);
            return slice.len;
        }
    };
}

pub fn instrIterator(reader: anytype) InstrIterator(@TypeOf(reader), 32) {
    return .{ .inner_reader = reader };
}

pub const Op = enum {
    aaa,
    aad,
    aam,
    aas,
    adc,
    add,
    @"and",
    call,
    callf,
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
    jmpf,
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
};

pub const Register = enum {
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

    comptime {
        // Order and value of fields are important to the getters!
        const expected =
            .{ "al", "cl", "dl", "bl", "ah", "ch", "dh", "bh", "ax", "cx", "dx", "bx", "sp", "bp", "si", "di", "es", "cs", "ss", "ds" };
        for (@typeInfo(@This()).Enum.fields, expected, 0..) |info, name, val| {
            std.debug.assert(std.mem.eql(u8, info.name, name));
            std.debug.assert(info.value == val);
        }
    }

    // In honour of gpanders! 🫡
    inline fn getGeneralPurpose(reg: u3, w: bool) Register {
        return @intToEnum(Register, @enumToInt(Register.ax) * @boolToInt(w) + reg);
    }

    inline fn getSegment(reg: u2) Register {
        return @intToEnum(Register, @enumToInt(Register.es) + reg);
    }
};

pub const Instr = struct {
    op: Op,
    payload: Payload,
    exclusive_prefix: ExclusivePrefix = .none,
    seg_override_prefix: SegOverridePrefix = .none,

    pub const ExclusivePrefix = enum { none, lock, rep, repne };
    pub const SegOverridePrefix = enum { none, @"cs:", @"ds:", @"es:", @"ss:" };

    pub const Payload = union(enum) {
        none,
        mod: ModInstr,
        mod_special: ModSpecialInstr,
        data: DataInstr,
        addr: AddrInstr,
        ip_off: i16,
        interseg_addr: struct { cs: u16, addr: u16 },
        regs: struct { dst: Register, src: Register },
        reg: Register,
        imm: u16,
    };
};

pub fn nextInstr(reader: anytype) !?Instr {
    var exclusive_prefix: Instr.ExclusivePrefix = .none;
    var seg_override_prefix: Instr.SegOverridePrefix = .none;
    const first = while (true) {
        const byte = reader.readByte() catch |err| switch (err) {
            error.EndOfStream => return if (exclusive_prefix != .none or seg_override_prefix != .none)
                err
            else
                null,
            else => return err,
        };

        switch (byte) {
            0x26 => seg_override_prefix = .@"es:",
            0x2e => seg_override_prefix = .@"cs:",
            0x36 => seg_override_prefix = .@"ss:",
            0x3e => seg_override_prefix = .@"ds:",
            0xf0 => exclusive_prefix = .lock,
            0xf2 => exclusive_prefix = .repne,
            0xf3 => exclusive_prefix = .rep,
            else => break byte,
        }
    };

    var second: ?u8 = null; // Need to read-ahead to identify some instructions.
    const op: Op = switch (first) {
        0x00...0x05 => .add,
        0x06, 0x0e, 0x16, 0x1e, 0x50...0x57 => .push,
        0x07, 0x17, 0x1f, 0x58...0x5f => .pop,
        0x08...0x0d => .@"or",
        0x10...0x15 => .adc,
        0x18...0x1d => .sbb,
        0x20...0x25 => .@"and",
        0x27 => .daa,
        0x28...0x2d => .sub,
        0x2f => .das,
        0x30...0x35 => .xor,
        0x37 => .aaa,
        0x38...0x3d => .cmp,
        0x3f => .aas,
        0x40...0x47 => .inc,
        0x48...0x4f => .dec,

        0xd0...0xd3 => blk: {
            second = try reader.readByte();
            break :blk switch (ModSpecialInstr.parseOp(second.?)) {
                0 => .rol,
                1 => .ror,
                2 => .rcl,
                3 => .rcr,
                4 => .sal,
                5 => .shr,
                7 => .sar,
                else => return error.UnknownInstr,
            };
        },

        0xd4...0xd5 => blk: {
            second = try reader.readByte();
            if (second.? != 0b1010) return error.UnknownInstr;
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
            second = try reader.readByte();
            break :blk switch (ModSpecialInstr.parseOp(second.?)) {
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
            second = try reader.readByte();
            if (ModSpecialInstr.parseOp(second.?) != 0) return error.UnknownInstr;
            break :blk .mov;
        },

        0x8d => .lea,

        0x8f => blk: {
            second = try reader.readByte();
            if (ModSpecialInstr.parseOp(second.?) != 0) return error.UnknownInstr;
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
        0xf4 => .hlt,
        0xf5 => .cmc,

        0xf6...0xf7 => blk: {
            second = try reader.readByte();
            break :blk switch (ModSpecialInstr.parseOp(second.?)) {
                0 => .@"test",
                2 => .not,
                3 => .neg,
                4 => .mul,
                5 => .imul,
                6 => .div,
                7 => .idiv,
                else => return error.UnknownInstr,
            };
        },

        0xf8 => .clc,
        0xf9 => .stc,
        0xfa => .cli,
        0xfb => .sti,
        0xfc => .cld,
        0xfd => .std,

        0xfe => blk: {
            second = try reader.readByte();
            break :blk switch (ModSpecialInstr.parseOp(second.?)) {
                0 => .inc,
                1 => .dec,
                else => return error.UnknownInstr,
            };
        },

        0xff => blk: {
            second = try reader.readByte();
            break :blk switch (ModSpecialInstr.parseOp(second.?)) {
                0 => .inc,
                1 => .dec,
                2 => .call,
                3 => .callf,
                4 => .jmp,
                5 => .jmpf,
                6 => .push,
                else => return error.UnknownInstr,
            };
        },

        else => return error.UnknownInstr,
    };

    const payload: Instr.Payload = if (second) |s| switch (op) {
        .aam, .aad => .none,
        else => .{ .mod_special = try ModSpecialInstr.decode(reader, first, s) },
    } else switch (first) {
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
        => .{ .mod = try ModInstr.decode(reader, first) },

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
        => .{ .data = try DataInstr.decode(reader, first) },

        0x06...0x07 => .{ .reg = .es },
        0x0e => .{ .reg = .cs },
        0x16...0x17 => .{ .reg = .ss },
        0x1e...0x1f => .{ .reg = .ds },
        0x40...0x5f => .{ .reg = Register.getGeneralPurpose(@intCast(u3, first & 7), true) },

        0x70...0x7f, 0xe0...0xe3, 0xeb => .{ .ip_off = try reader.readByteSigned() },
        0xe8...0xe9 => .{ .ip_off = try reader.readIntLittle(i16) },

        0x91...0x97 => .{ .regs = .{
            .dst = .ax,
            .src = Register.getGeneralPurpose(@intCast(u3, first & 7), true),
        } },

        0x9a, 0xea => .{ .interseg_addr = .{
            .addr = try reader.readIntLittle(u16),
            .cs = try reader.readIntLittle(u16),
        } },

        0xa0...0xa3 => .{ .addr = try AddrInstr.decode(reader, first) },

        0xc2, 0xca => .{ .imm = try reader.readIntLittle(u16) },
        0xcc => .{ .imm = 3 },
        0xcd => .{ .imm = try reader.readByte() },

        0xec...0xef => .{ .regs = .{
            .dst = if (first & 1 == 1) .ax else .al,
            .src = .dx,
        } },

        else => .none,
    };

    return .{
        .op = op,
        .payload = payload,
        .exclusive_prefix = exclusive_prefix,
        .seg_override_prefix = seg_override_prefix,
    };
}

pub const ModInstr = struct {
    w: bool,
    d: bool,
    reg: Register,
    operand: ModOperand,

    pub inline fn getDst(self: ModInstr) ModOperand {
        return if (self.d) .{ .reg = self.reg } else self.operand;
    }

    pub inline fn getSrc(self: ModInstr) ModOperand {
        return if (self.d) self.operand else .{ .reg = self.reg };
    }

    fn decode(reader: anytype, first: u8) !ModInstr {
        const info: struct { w: bool, d: bool } = switch (first) {
            0x8c => .{ .w = true, .d = (first >> 1) & 1 == 1 },
            0x8d, 0x8e, 0xc4, 0xc5 => .{ .w = true, .d = true },
            else => .{ .w = first & 1 == 1, .d = (first >> 1) & 1 == 1 },
        };

        const second = try reader.readByte();
        const mod = @intCast(u2, (second >> 6) & 0b11);
        const rm = @intCast(u3, second & 0b111);
        const reg = @intCast(u3, (second >> 3) & 0b111);
        const decoded_reg = switch (first) {
            0x8c, 0x8e => if ((reg >> 2) & 1 == 0)
                Register.getSegment(@intCast(u2, reg))
            else
                return error.UnknownInstr,
            else => Register.getGeneralPurpose(reg, info.w),
        };
        const operand = try ModOperand.decode(reader, mod, rm, info.w);

        return .{ .w = info.w, .d = info.d, .reg = decoded_reg, .operand = operand };
    }
};

pub const ModSpecialInstr = struct {
    w: bool,
    dst: ModOperand,
    src: SrcOperand,

    pub const SrcOperand = union(enum) {
        none,
        cl,
        uimm: u16,
        simm: i8,
    };

    fn decode(reader: anytype, first: u8, second: u8) !ModSpecialInstr {
        const w = first & 1 == 1;
        const mod = @intCast(u2, (second >> 6) & 0b11);
        const rm = @intCast(u3, second & 0b111);
        const dst = try ModOperand.decode(reader, mod, rm, w);

        const src_type: @typeInfo(SrcOperand).Union.tag_type.? = switch (first) {
            0x83 => .simm,
            0x8f, 0xfe, 0xff => .none,
            0xd2, 0xd3 => .cl,
            0xf6, 0xf7 => if (parseOp(second) == 0) .uimm else .none,
            else => .uimm,
        };
        const src: ModSpecialInstr.SrcOperand = switch (src_type) {
            .uimm => switch (first) {
                0xd0, 0xd1 => .{ .uimm = 1 },
                else => .{ .uimm = if (w) try reader.readIntLittle(u16) else try reader.readByte() },
            },
            .simm => .{ .simm = try reader.readByteSigned() },
            inline else => |tag| tag,
        };

        return .{ .w = w, .dst = dst, .src = src };
    }

    inline fn parseOp(second: u8) u3 {
        return @intCast(u3, (second >> 3) & 0b111);
    }
};

pub const ModOperand = union(enum) {
    reg: Register,
    addr: EffectiveAddr,

    fn decode(reader: anytype, mod: u2, rm: u3, w: bool) !ModOperand {
        return switch (mod) {
            0 => .{ .addr = EffectiveAddr.decode(mod, rm, if (rm == 0b110) try reader.readIntLittle(u16) else 0) },
            1 => .{ .addr = EffectiveAddr.decode(mod, rm, try reader.readByte()) },
            2 => .{ .addr = EffectiveAddr.decode(mod, rm, try reader.readIntLittle(u16)) },
            3 => .{ .reg = Register.getGeneralPurpose(rm, w) },
        };
    }
};

pub const EffectiveAddr = struct {
    regs: Registers = .none,
    off: u16 = 0,

    pub const Registers = enum {
        @"bx+si",
        @"bx+di",
        @"bp+si",
        @"bp+di",
        si,
        di,
        bp,
        bx,
        none,

        comptime {
            // Order and value of register fields are important to decode()!
            const expected_regs = .{ "bx+si", "bx+di", "bp+si", "bp+di", "si", "di", "bp", "bx" };
            for (@typeInfo(@This()).Enum.fields[0..expected_regs.len], expected_regs, 0..) |info, name, val| {
                std.debug.assert(std.mem.eql(u8, info.name, name));
                std.debug.assert(info.value == val);
            }
        }
    };

    fn decode(mod: u2, rm: u3, off: u16) EffectiveAddr {
        return switch (mod) {
            0 => return if (rm == 0b110)
                .{ .off = off }
            else
                .{ .regs = @intToEnum(EffectiveAddr.Registers, rm) },
            1, 2 => .{ .regs = @intToEnum(EffectiveAddr.Registers, rm), .off = off },
            3 => unreachable,
        };
    }
};

pub const DataInstr = struct {
    dst_reg: Register,
    imm: u16,

    fn decode(reader: anytype, first: u8) !DataInstr {
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
            .dst_reg = Register.getGeneralPurpose(info.reg, info.regw),
            .imm = if (info.dataw) try reader.readIntLittle(u16) else try reader.readByte(),
        };
    }
};

pub const AddrInstr = struct {
    w: bool,
    d: bool,
    addr: u16,

    pub const Operand = union(enum) {
        reg: Register,
        addr: u16,
    };

    pub inline fn getDst(self: AddrInstr) Operand {
        return if (self.d) .{ .addr = self.addr } else .{ .reg = if (self.w) .ax else .al };
    }

    pub inline fn getSrc(self: AddrInstr) Operand {
        return if (self.d) .{ .reg = if (self.w) .ax else .al } else .{ .addr = self.addr };
    }

    fn decode(reader: anytype, first: u8) !AddrInstr {
        return .{
            .w = first & 1 == 1,
            .d = (first >> 1) & 1 == 1,
            .addr = try reader.readIntLittle(u16),
        };
    }
};

test InstrIterator {
    {
        // Empty stream is valid.
        var stream = std.io.fixedBufferStream("");
        var it = instrIterator(stream.reader());
        try std.testing.expectEqual(@as(?Instr, null), try it.next());
        try std.testing.expectEqualSlices(u8, &.{}, it.getPrevBytes());
    }
    {
        var stream = std.io.fixedBufferStream("\x80");
        var it = instrIterator(stream.reader());
        try std.testing.expectError(error.EndOfStream, it.next());
        try std.testing.expectEqualSlices(u8, &.{0x80}, it.getPrevBytes());
    }
    {
        var stream = std.io.fixedBufferStream("\xf0");
        var it = instrIterator(stream.reader());
        try std.testing.expectError(error.EndOfStream, it.next());
        try std.testing.expectEqualSlices(u8, &.{0xf0}, it.getPrevBytes());
    }
    {
        var stream = std.io.fixedBufferStream("\xd9\x13");
        var it = instrIterator(stream.reader());
        try std.testing.expectError(error.UnknownInstr, it.next());
        try std.testing.expectEqualSlices(u8, &.{0xd9}, it.getPrevBytes());
    }
    {
        var stream = std.io.fixedBufferStream("\xd4\x99");
        var it = instrIterator(stream.reader());
        try std.testing.expectError(error.UnknownInstr, it.next());
        try std.testing.expectEqualSlices(u8, &.{ 0xd4, 0x99 }, it.getPrevBytes());
    }
    {
        var stream = std.io.fixedBufferStream(&[_]u8{ 0xd3, 0b10_110_001 });
        var it = instrIterator(stream.reader());
        try std.testing.expectError(error.UnknownInstr, it.next());
        try std.testing.expectEqualSlices(u8, &.{ 0xd3, 0b10_110_001 }, it.getPrevBytes());
    }
    {
        var stream = std.io.fixedBufferStream(&[_]u8{ 0xd3, 0b10_111_001 });
        var it = instrIterator(stream.reader());
        try std.testing.expectError(error.EndOfStream, it.next());
        try std.testing.expectEqualSlices(u8, &.{ 0xd3, 0b10_111_001 }, it.getPrevBytes());
    }
    {
        var stream = std.io.fixedBufferStream(&[_]u8{ 0xd3, 0b10_111_001, 0x30 });
        var it = instrIterator(stream.reader());
        try std.testing.expectError(error.EndOfStream, it.next());
        try std.testing.expectEqualSlices(u8, &.{ 0xd3, 0b10_111_001, 0x30 }, it.getPrevBytes());
    }
    {
        var stream = std.io.fixedBufferStream(&[_]u8{ 0xd3, 0b10_111_001, 0x30, 0x40 });
        var it = instrIterator(stream.reader());
        try testExpectModSpecialInstr(.sar, .{ .addr = .{ .regs = .@"bx+di", .off = 16432 } }, .cl, true, try it.next());
        try std.testing.expectEqualSlices(u8, &.{ 0xd3, 0b10_111_001, 0x30, 0x40 }, it.getPrevBytes());
        try std.testing.expectEqual(@as(?Instr, null), try it.next());
        try std.testing.expectEqualSlices(u8, &.{}, it.getPrevBytes());
    }
}

// Following tests are based on the "Computer, Enhance!" perfaware listings:
// https://github.com/cmuratori/computer_enhance/blob/main/perfaware/part1
const testOpenListing = @import("test.zig").testOpenListing;

test "0037: single register mov" {
    var file = try testOpenListing("0037_single_register_mov");
    defer file.close();
    const reader = file.reader();

    try testExpectModInstr(.mov, .{ .reg = .cx }, .{ .reg = .bx }, try nextInstr(reader));
    try testing.expectEqual(@as(?Instr, null), try nextInstr(reader));
}

test "0038: many register mov" {
    var file = try testOpenListing("0038_many_register_mov");
    defer file.close();
    const reader = file.reader();

    try testExpectModInstr(.mov, .{ .reg = .cx }, .{ .reg = .bx }, try nextInstr(reader));
    try testExpectModInstr(.mov, .{ .reg = .ch }, .{ .reg = .ah }, try nextInstr(reader));
    try testExpectModInstr(.mov, .{ .reg = .dx }, .{ .reg = .bx }, try nextInstr(reader));
    try testExpectModInstr(.mov, .{ .reg = .si }, .{ .reg = .bx }, try nextInstr(reader));
    try testExpectModInstr(.mov, .{ .reg = .bx }, .{ .reg = .di }, try nextInstr(reader));
    try testExpectModInstr(.mov, .{ .reg = .al }, .{ .reg = .cl }, try nextInstr(reader));
    try testExpectModInstr(.mov, .{ .reg = .ch }, .{ .reg = .ch }, try nextInstr(reader));
    try testExpectModInstr(.mov, .{ .reg = .bx }, .{ .reg = .ax }, try nextInstr(reader));
    try testExpectModInstr(.mov, .{ .reg = .bx }, .{ .reg = .si }, try nextInstr(reader));
    try testExpectModInstr(.mov, .{ .reg = .sp }, .{ .reg = .di }, try nextInstr(reader));
    try testExpectModInstr(.mov, .{ .reg = .bp }, .{ .reg = .ax }, try nextInstr(reader));
    try testing.expectEqual(@as(?Instr, null), try nextInstr(reader));
}

test "0039: more movs" {
    var file = try testOpenListing("0039_more_movs");
    defer file.close();
    const reader = file.reader();

    // Register-to-register.
    try testExpectModInstr(.mov, .{ .reg = .si }, .{ .reg = .bx }, try nextInstr(reader));
    try testExpectModInstr(.mov, .{ .reg = .dh }, .{ .reg = .al }, try nextInstr(reader));

    // 8-bit intermediate-to-register.
    try testExpectDataInstr(.mov, .cl, 12, try nextInstr(reader));
    try testExpectDataInstr(.mov, .ch, @bitCast(u8, @as(i8, -12)), try nextInstr(reader));

    // 16-bit intermediate-to-register.
    try testExpectDataInstr(.mov, .cx, 12, try nextInstr(reader));
    try testExpectDataInstr(.mov, .cx, @bitCast(u16, @as(i16, -12)), try nextInstr(reader));
    try testExpectDataInstr(.mov, .dx, 3948, try nextInstr(reader));
    try testExpectDataInstr(.mov, .dx, @bitCast(u16, @as(i16, -3948)), try nextInstr(reader));

    // Source address calculation.
    try testExpectModInstr(.mov, .{ .reg = .al }, .{ .addr = .{ .regs = .@"bx+si" } }, try nextInstr(reader));
    try testExpectModInstr(.mov, .{ .reg = .bx }, .{ .addr = .{ .regs = .@"bp+di" } }, try nextInstr(reader));
    try testExpectModInstr(.mov, .{ .reg = .dx }, .{ .addr = .{ .regs = .bp } }, try nextInstr(reader));

    // Source address calculation plus 8-bit displacement.
    try testExpectModInstr(.mov, .{ .reg = .ah }, .{ .addr = .{ .regs = .@"bx+si", .off = 4 } }, try nextInstr(reader));

    // Source address calculation plus 16-bit displacement.
    try testExpectModInstr(.mov, .{ .reg = .al }, .{ .addr = .{ .regs = .@"bx+si", .off = 4999 } }, try nextInstr(reader));

    // Destination address calculation.
    try testExpectModInstr(.mov, .{ .addr = .{ .regs = .@"bx+di" } }, .{ .reg = .cx }, try nextInstr(reader));
    try testExpectModInstr(.mov, .{ .addr = .{ .regs = .@"bp+si" } }, .{ .reg = .cl }, try nextInstr(reader));
    try testExpectModInstr(.mov, .{ .addr = .{ .regs = .bp } }, .{ .reg = .ch }, try nextInstr(reader));

    try testing.expectEqual(@as(?Instr, null), try nextInstr(reader));
}

test "0040: challenge movs" {
    var file = try testOpenListing("0040_challenge_movs");
    defer file.close();
    const reader = file.reader();

    // Signed displacements.
    try testExpectModInstr(
        .mov,
        .{ .reg = .ax },
        .{ .addr = .{ .regs = .@"bx+di", .off = @bitCast(u8, @as(i8, -37)) } },
        try nextInstr(reader),
    );
    try testExpectModInstr(
        .mov,
        .{ .addr = .{ .regs = .si, .off = @bitCast(u16, @as(i16, -300)) } },
        .{ .reg = .cx },
        try nextInstr(reader),
    );
    try testExpectModInstr(
        .mov,
        .{ .reg = .dx },
        .{ .addr = .{ .regs = .bx, .off = @bitCast(u8, @as(i8, -32)) } },
        try nextInstr(reader),
    );

    // Explicit sizes.
    try testExpectModSpecialInstr(.mov, .{ .addr = .{ .regs = .@"bp+di" } }, .{ .uimm = 7 }, false, try nextInstr(reader));
    try testExpectModSpecialInstr(.mov, .{ .addr = .{ .regs = .di, .off = 901 } }, .{ .uimm = 347 }, true, try nextInstr(reader));

    // Direct address.
    try testExpectModInstr(.mov, .{ .reg = .bp }, .{ .addr = .{ .off = 5 } }, try nextInstr(reader));
    try testExpectModInstr(.mov, .{ .reg = .bx }, .{ .addr = .{ .off = 3458 } }, try nextInstr(reader));

    // Memory-to-accumulator test.
    try testExpectAddrInstr(.mov, .{ .reg = .ax }, .{ .addr = 2555 }, try nextInstr(reader));
    try testExpectAddrInstr(.mov, .{ .reg = .ax }, .{ .addr = 16 }, try nextInstr(reader));

    // Accumulator-to-memory test.
    try testExpectAddrInstr(.mov, .{ .addr = 2554 }, .{ .reg = .ax }, try nextInstr(reader));
    try testExpectAddrInstr(.mov, .{ .addr = 15 }, .{ .reg = .ax }, try nextInstr(reader));

    try testing.expectEqual(@as(?Instr, null), try nextInstr(reader));
}

test "0041: add sub cmp jnz" {
    var file = try testOpenListing("0041_add_sub_cmp_jnz");
    defer file.close();
    const reader = file.reader();

    for ([_]Op{ .add, .sub, .cmp }) |op| {
        try testExpectModInstr(op, .{ .reg = .bx }, .{ .addr = .{ .regs = .@"bx+si" } }, try nextInstr(reader));
        try testExpectModInstr(op, .{ .reg = .bx }, .{ .addr = .{ .regs = .bp } }, try nextInstr(reader));
        try testExpectModSpecialInstr(op, .{ .reg = .si }, .{ .simm = 2 }, true, try nextInstr(reader));
        try testExpectModSpecialInstr(op, .{ .reg = .bp }, .{ .simm = 2 }, true, try nextInstr(reader));
        try testExpectModSpecialInstr(op, .{ .reg = .cx }, .{ .simm = 8 }, true, try nextInstr(reader));
        try testExpectModInstr(op, .{ .reg = .bx }, .{ .addr = .{ .regs = .bp } }, try nextInstr(reader));
        try testExpectModInstr(op, .{ .reg = .cx }, .{ .addr = .{ .regs = .bx, .off = 2 } }, try nextInstr(reader));
        try testExpectModInstr(op, .{ .reg = .bh }, .{ .addr = .{ .regs = .@"bp+si", .off = 4 } }, try nextInstr(reader));
        try testExpectModInstr(op, .{ .reg = .di }, .{ .addr = .{ .regs = .@"bp+di", .off = 6 } }, try nextInstr(reader));
        try testExpectModInstr(op, .{ .addr = .{ .regs = .@"bx+si" } }, .{ .reg = .bx }, try nextInstr(reader));
        try testExpectModInstr(op, .{ .addr = .{ .regs = .bp } }, .{ .reg = .bx }, try nextInstr(reader));
        try testExpectModInstr(op, .{ .addr = .{ .regs = .bp } }, .{ .reg = .bx }, try nextInstr(reader));
        try testExpectModInstr(op, .{ .addr = .{ .regs = .bx, .off = 2 } }, .{ .reg = .cx }, try nextInstr(reader));
        try testExpectModInstr(op, .{ .addr = .{ .regs = .@"bp+si", .off = 4 } }, .{ .reg = .bh }, try nextInstr(reader));
        try testExpectModInstr(op, .{ .addr = .{ .regs = .@"bp+di", .off = 6 } }, .{ .reg = .di }, try nextInstr(reader));
        try testExpectModSpecialInstr(op, .{ .addr = .{ .regs = .bx } }, .{ .uimm = 34 }, false, try nextInstr(reader));

        {
            const addr: EffectiveAddr = switch (op) {
                .add => .{ .regs = .@"bp+si", .off = 1000 },
                .sub => .{ .regs = .@"bx+di" },
                .cmp => .{ .off = 4834 },
                else => unreachable,
            };
            try testExpectModSpecialInstr(op, .{ .addr = addr }, .{ .simm = 29 }, true, try nextInstr(reader));
        }

        try testExpectModInstr(op, .{ .reg = .ax }, .{ .addr = .{ .regs = .bp } }, try nextInstr(reader));
        try testExpectModInstr(op, .{ .reg = .al }, .{ .addr = .{ .regs = .@"bx+si" } }, try nextInstr(reader));
        try testExpectModInstr(op, .{ .reg = .ax }, .{ .reg = .bx }, try nextInstr(reader));
        try testExpectModInstr(op, .{ .reg = .al }, .{ .reg = .ah }, try nextInstr(reader));
        try testExpectDataInstr(op, .ax, 1000, try nextInstr(reader));
        try testExpectDataInstr(op, .al, @bitCast(u8, @as(i8, -30)), try nextInstr(reader));
        try testExpectDataInstr(op, .al, 9, try nextInstr(reader));
    }

    try testing.expectEqual(Instr{ .op = .jne, .payload = .{ .ip_off = 2 } }, (try nextInstr(reader)).?);
    try testing.expectEqual(Instr{ .op = .jne, .payload = .{ .ip_off = -4 } }, (try nextInstr(reader)).?);
    try testing.expectEqual(Instr{ .op = .jne, .payload = .{ .ip_off = -6 } }, (try nextInstr(reader)).?);
    try testing.expectEqual(Instr{ .op = .jne, .payload = .{ .ip_off = -4 } }, (try nextInstr(reader)).?);

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
            Instr{ .op = op, .payload = .{ .ip_off = -2 * @intCast(i16, off) } },
            (try nextInstr(reader)).?,
        );
    }

    try testing.expectEqual(@as(?Instr, null), try nextInstr(reader));
}

test "0042: completionist decode" {
    var file = try testOpenListing("0042_completionist_decode");
    defer file.close();
    const reader = file.reader();

    try testExpectModInstr(.mov, .{ .reg = .si }, .{ .reg = .bx }, try nextInstr(reader));
    try testExpectModInstr(.mov, .{ .reg = .dh }, .{ .reg = .al }, try nextInstr(reader));
    try testExpectDataInstr(.mov, .cl, 12, try nextInstr(reader));
    try testExpectDataInstr(.mov, .ch, @bitCast(u8, @as(i8, -12)), try nextInstr(reader));
    try testExpectDataInstr(.mov, .cx, 12, try nextInstr(reader));
    try testExpectDataInstr(.mov, .cx, @bitCast(u16, @as(i16, -12)), try nextInstr(reader));
    try testExpectDataInstr(.mov, .dx, 3948, try nextInstr(reader));
    try testExpectDataInstr(.mov, .dx, @bitCast(u16, @as(i16, -3948)), try nextInstr(reader));
    try testExpectModInstr(.mov, .{ .reg = .al }, .{ .addr = .{ .regs = .@"bx+si" } }, try nextInstr(reader));
    try testExpectModInstr(.mov, .{ .reg = .bx }, .{ .addr = .{ .regs = .@"bp+di" } }, try nextInstr(reader));
    try testExpectModInstr(.mov, .{ .reg = .dx }, .{ .addr = .{ .regs = .bp } }, try nextInstr(reader));
    try testExpectModInstr(.mov, .{ .reg = .ah }, .{ .addr = .{ .regs = .@"bx+si", .off = 4 } }, try nextInstr(reader));
    try testExpectModInstr(.mov, .{ .reg = .al }, .{ .addr = .{ .regs = .@"bx+si", .off = 4999 } }, try nextInstr(reader));
    try testExpectModInstr(.mov, .{ .addr = .{ .regs = .@"bx+di" } }, .{ .reg = .cx }, try nextInstr(reader));
    try testExpectModInstr(.mov, .{ .addr = .{ .regs = .@"bp+si" } }, .{ .reg = .cl }, try nextInstr(reader));
    try testExpectModInstr(.mov, .{ .addr = .{ .regs = .bp } }, .{ .reg = .ch }, try nextInstr(reader));
    try testExpectModInstr(
        .mov,
        .{ .reg = .ax },
        .{ .addr = .{ .regs = .@"bx+di", .off = @bitCast(u8, @as(i8, -37)) } },
        try nextInstr(reader),
    );
    try testExpectModInstr(
        .mov,
        .{ .addr = .{ .regs = .si, .off = @bitCast(u16, @as(i16, -300)) } },
        .{ .reg = .cx },
        try nextInstr(reader),
    );
    try testExpectModInstr(
        .mov,
        .{ .reg = .dx },
        .{ .addr = .{ .regs = .bx, .off = @bitCast(u8, @as(i8, -32)) } },
        try nextInstr(reader),
    );
    try testExpectModSpecialInstr(.mov, .{ .addr = .{ .regs = .@"bp+di" } }, .{ .uimm = 7 }, false, try nextInstr(reader));
    try testExpectModSpecialInstr(.mov, .{ .addr = .{ .regs = .di, .off = 901 } }, .{ .uimm = 347 }, true, try nextInstr(reader));
    try testExpectModInstr(.mov, .{ .reg = .bp }, .{ .addr = .{ .off = 5 } }, try nextInstr(reader));
    try testExpectModInstr(.mov, .{ .reg = .bx }, .{ .addr = .{ .off = 3458 } }, try nextInstr(reader));
    try testExpectAddrInstr(.mov, .{ .reg = .ax }, .{ .addr = 2555 }, try nextInstr(reader));
    try testExpectAddrInstr(.mov, .{ .reg = .ax }, .{ .addr = 16 }, try nextInstr(reader));
    try testExpectAddrInstr(.mov, .{ .addr = 2554 }, .{ .reg = .ax }, try nextInstr(reader));
    try testExpectAddrInstr(.mov, .{ .addr = 15 }, .{ .reg = .ax }, try nextInstr(reader));

    try testExpectModSpecialInstr(.push, .{ .addr = .{ .regs = .@"bp+si" } }, .none, true, try nextInstr(reader));
    try testExpectModSpecialInstr(.push, .{ .addr = .{ .off = 3000 } }, .none, true, try nextInstr(reader));
    try testExpectModSpecialInstr(
        .push,
        .{ .addr = .{ .regs = .@"bx+di", .off = @bitCast(u8, @as(i8, -30)) } },
        .none,
        true,
        try nextInstr(reader),
    );
    try testing.expectEqual(Instr{ .op = .push, .payload = .{ .reg = .cx } }, (try nextInstr(reader)).?);
    try testing.expectEqual(Instr{ .op = .push, .payload = .{ .reg = .ax } }, (try nextInstr(reader)).?);
    try testing.expectEqual(Instr{ .op = .push, .payload = .{ .reg = .dx } }, (try nextInstr(reader)).?);
    try testing.expectEqual(Instr{ .op = .push, .payload = .{ .reg = .cs } }, (try nextInstr(reader)).?);

    try testExpectModSpecialInstr(.pop, .{ .addr = .{ .regs = .@"bp+si" } }, .none, true, try nextInstr(reader));
    try testExpectModSpecialInstr(.pop, .{ .addr = .{ .off = 3 } }, .none, true, try nextInstr(reader));
    try testExpectModSpecialInstr(
        .pop,
        .{ .addr = .{ .regs = .@"bx+di", .off = @bitCast(u16, @as(i16, -3000)) } },
        .none,
        true,
        try nextInstr(reader),
    );
    try testing.expectEqual(Instr{ .op = .pop, .payload = .{ .reg = .sp } }, (try nextInstr(reader)).?);
    try testing.expectEqual(Instr{ .op = .pop, .payload = .{ .reg = .di } }, (try nextInstr(reader)).?);
    try testing.expectEqual(Instr{ .op = .pop, .payload = .{ .reg = .si } }, (try nextInstr(reader)).?);
    try testing.expectEqual(Instr{ .op = .pop, .payload = .{ .reg = .ds } }, (try nextInstr(reader)).?);

    try testExpectModInstr(
        .xchg,
        .{ .reg = .ax },
        .{ .addr = .{ .regs = .bp, .off = @bitCast(u16, @as(i16, -1000)) } },
        try nextInstr(reader),
    );
    try testExpectModInstr(.xchg, .{ .reg = .bp }, .{ .addr = .{ .regs = .bx, .off = 50 } }, try nextInstr(reader));

    try testing.expectEqual(Instr{ .op = .nop, .payload = .none }, (try nextInstr(reader)).?);
    try testing.expectEqual(
        Instr{ .op = .xchg, .payload = .{ .regs = .{ .dst = .ax, .src = .dx } } },
        (try nextInstr(reader)).?,
    );
    try testing.expectEqual(
        Instr{ .op = .xchg, .payload = .{ .regs = .{ .dst = .ax, .src = .sp } } },
        (try nextInstr(reader)).?,
    );
    try testing.expectEqual(
        Instr{ .op = .xchg, .payload = .{ .regs = .{ .dst = .ax, .src = .si } } },
        (try nextInstr(reader)).?,
    );
    try testing.expectEqual(
        Instr{ .op = .xchg, .payload = .{ .regs = .{ .dst = .ax, .src = .di } } },
        (try nextInstr(reader)).?,
    );

    try testExpectModInstr(.xchg, .{ .reg = .cx }, .{ .reg = .dx }, try nextInstr(reader));
    try testExpectModInstr(.xchg, .{ .reg = .si }, .{ .reg = .cx }, try nextInstr(reader));
    try testExpectModInstr(.xchg, .{ .reg = .cl }, .{ .reg = .ah }, try nextInstr(reader));

    try testExpectDataInstr(.in, .al, 200, try nextInstr(reader));
    try testing.expectEqual(
        Instr{ .op = .in, .payload = .{ .regs = .{ .dst = .al, .src = .dx } } },
        (try nextInstr(reader)).?,
    );
    try testing.expectEqual(
        Instr{ .op = .in, .payload = .{ .regs = .{ .dst = .ax, .src = .dx } } },
        (try nextInstr(reader)).?,
    );

    try testExpectDataInstr(.out, .ax, 44, try nextInstr(reader));
    try testing.expectEqual(
        Instr{ .op = .out, .payload = .{ .regs = .{ .dst = .al, .src = .dx } } },
        (try nextInstr(reader)).?,
    );

    try testing.expectEqual(Instr{ .op = .xlat, .payload = .none }, (try nextInstr(reader)).?);

    for ([_]Op{ .lea, .lds, .les }) |op| {
        try testExpectModInstr(op, .{ .reg = .ax }, .{ .addr = .{ .regs = .@"bx+di", .off = 1420 } }, try nextInstr(reader));
        try testExpectModInstr(
            op,
            .{ .reg = .bx },
            .{ .addr = .{ .regs = .bp, .off = @bitCast(u8, @as(i8, -50)) } },
            try nextInstr(reader),
        );
        try testExpectModInstr(
            op,
            .{ .reg = .sp },
            .{ .addr = .{ .regs = .bp, .off = @bitCast(u16, @as(i16, -1003)) } },
            try nextInstr(reader),
        );
        try testExpectModInstr(
            op,
            .{ .reg = .di },
            .{ .addr = .{ .regs = .@"bx+si", .off = @bitCast(u8, @as(i8, -7)) } },
            try nextInstr(reader),
        );
    }

    try testing.expectEqual(Instr{ .op = .lahf, .payload = .none }, (try nextInstr(reader)).?);
    try testing.expectEqual(Instr{ .op = .sahf, .payload = .none }, (try nextInstr(reader)).?);
    try testing.expectEqual(Instr{ .op = .pushf, .payload = .none }, (try nextInstr(reader)).?);
    try testing.expectEqual(Instr{ .op = .popf, .payload = .none }, (try nextInstr(reader)).?);

    const testAddSub = struct {
        fn f(r: anytype, op: Op) !void {
            try testExpectModInstr(op, .{ .reg = .cx }, .{ .addr = .{ .regs = .bp } }, try nextInstr(r));
            try testExpectModInstr(op, .{ .reg = .dx }, .{ .addr = .{ .regs = .@"bx+si" } }, try nextInstr(r));
            try testExpectModInstr(op, .{ .addr = .{ .regs = .@"bp+di", .off = 5000 } }, .{ .reg = .ah }, try nextInstr(r));
            try testExpectModInstr(op, .{ .addr = .{ .regs = .bx } }, .{ .reg = .al }, try nextInstr(r));
            try testExpectModSpecialInstr(op, .{ .reg = .sp }, .{ .uimm = 392 }, true, try nextInstr(r));
            try testExpectModSpecialInstr(op, .{ .reg = .si }, .{ .simm = 5 }, true, try nextInstr(r));
            try testExpectDataInstr(op, .ax, 1000, try nextInstr(r));
            try testExpectModSpecialInstr(op, .{ .reg = .ah }, .{ .uimm = 30 }, false, try nextInstr(r));
            try testExpectDataInstr(op, .al, 9, try nextInstr(r));
            try testExpectModInstr(op, .{ .reg = .cx }, .{ .reg = .bx }, try nextInstr(r));
            try testExpectModInstr(op, .{ .reg = .ch }, .{ .reg = .al }, try nextInstr(r));
        }
    }.f;

    try testAddSub(reader, .add);
    try testAddSub(reader, .adc);

    const testIncDecNeg = struct {
        fn f(r: anytype, op: Op) !void {
            if (op == .neg) {
                try testExpectModSpecialInstr(op, .{ .reg = .ax }, .none, true, try nextInstr(r));
                try testExpectModSpecialInstr(op, .{ .reg = .cx }, .none, true, try nextInstr(r));
            } else {
                try testing.expectEqual(Instr{ .op = op, .payload = .{ .reg = .ax } }, (try nextInstr(r)).?);
                try testing.expectEqual(Instr{ .op = op, .payload = .{ .reg = .cx } }, (try nextInstr(r)).?);
            }

            try testExpectModSpecialInstr(op, .{ .reg = .dh }, .none, false, try nextInstr(r));
            try testExpectModSpecialInstr(op, .{ .reg = .al }, .none, false, try nextInstr(r));
            try testExpectModSpecialInstr(op, .{ .reg = .ah }, .none, false, try nextInstr(r));

            if (op == .neg) {
                try testExpectModSpecialInstr(op, .{ .reg = .sp }, .none, true, try nextInstr(r));
                try testExpectModSpecialInstr(op, .{ .reg = .di }, .none, true, try nextInstr(r));
            } else {
                try testing.expectEqual(Instr{ .op = op, .payload = .{ .reg = .sp } }, (try nextInstr(r)).?);
                try testing.expectEqual(Instr{ .op = op, .payload = .{ .reg = .di } }, (try nextInstr(r)).?);
            }

            try testExpectModSpecialInstr(op, .{ .addr = .{ .regs = .bp, .off = 1002 } }, .none, false, try nextInstr(r));
            try testExpectModSpecialInstr(op, .{ .addr = .{ .regs = .bx, .off = 39 } }, .none, true, try nextInstr(r));
            try testExpectModSpecialInstr(op, .{ .addr = .{ .regs = .@"bx+si", .off = 5 } }, .none, false, try nextInstr(r));
            try testExpectModSpecialInstr(
                op,
                .{ .addr = .{ .regs = .@"bp+di", .off = @bitCast(u16, @as(i16, -10044)) } },
                .none,
                true,
                try nextInstr(r),
            );
            try testExpectModSpecialInstr(op, .{ .addr = .{ .off = 9349 } }, .none, true, try nextInstr(r));
            try testExpectModSpecialInstr(op, .{ .addr = .{ .regs = .bp } }, .none, false, try nextInstr(r));
        }
    }.f;

    try testIncDecNeg(reader, .inc);

    try testing.expectEqual(Instr{ .op = .aaa, .payload = .none }, (try nextInstr(reader)).?);
    try testing.expectEqual(Instr{ .op = .daa, .payload = .none }, (try nextInstr(reader)).?);

    try testAddSub(reader, .sub);
    try testAddSub(reader, .sbb);

    try testIncDecNeg(reader, .dec);
    try testIncDecNeg(reader, .neg);

    const testCmpTest = struct {
        fn f(r: anytype, op: Op) !void {
            try testExpectModInstr(op, .{ .reg = .bx }, .{ .reg = .cx }, try nextInstr(r));

            if (op == .cmp) {
                try testExpectModInstr(op, .{ .reg = .dh }, .{ .addr = .{ .regs = .bp, .off = 390 } }, try nextInstr(r));
            } else {
                try testExpectModInstr(op, .{ .addr = .{ .regs = .bp, .off = 390 } }, .{ .reg = .dh }, try nextInstr(r));
            }

            try testExpectModInstr(op, .{ .addr = .{ .regs = .bp, .off = 2 } }, .{ .reg = .si }, try nextInstr(r));
            try testExpectModSpecialInstr(op, .{ .reg = .bl }, .{ .uimm = 20 }, false, try nextInstr(r));
            try testExpectModSpecialInstr(op, .{ .addr = .{ .regs = .bx } }, .{ .uimm = 34 }, false, try nextInstr(r));
            try testExpectDataInstr(op, .ax, 23909, try nextInstr(r));
        }
    }.f;

    try testCmpTest(reader, .cmp);

    try testing.expectEqual(Instr{ .op = .aas, .payload = .none }, (try nextInstr(reader)).?);
    try testing.expectEqual(Instr{ .op = .das, .payload = .none }, (try nextInstr(reader)).?);

    try testExpectModSpecialInstr(.mul, .{ .reg = .al }, .none, false, try nextInstr(reader));
    try testExpectModSpecialInstr(.mul, .{ .reg = .cx }, .none, true, try nextInstr(reader));
    try testExpectModSpecialInstr(.mul, .{ .addr = .{ .regs = .bp } }, .none, true, try nextInstr(reader));
    try testExpectModSpecialInstr(.mul, .{ .addr = .{ .regs = .@"bx+di", .off = 500 } }, .none, false, try nextInstr(reader));

    try testExpectModSpecialInstr(.imul, .{ .reg = .ch }, .none, false, try nextInstr(reader));
    try testExpectModSpecialInstr(.imul, .{ .reg = .dx }, .none, true, try nextInstr(reader));
    try testExpectModSpecialInstr(.imul, .{ .addr = .{ .regs = .bx } }, .none, false, try nextInstr(reader));
    try testExpectModSpecialInstr(.imul, .{ .addr = .{ .off = 9483 } }, .none, true, try nextInstr(reader));

    try testing.expectEqual(Instr{ .op = .aam, .payload = .none }, (try nextInstr(reader)).?);

    try testExpectModSpecialInstr(.div, .{ .reg = .bl }, .none, false, try nextInstr(reader));
    try testExpectModSpecialInstr(.div, .{ .reg = .sp }, .none, true, try nextInstr(reader));
    try testExpectModSpecialInstr(.div, .{ .addr = .{ .regs = .@"bx+si", .off = 2990 } }, .none, false, try nextInstr(reader));
    try testExpectModSpecialInstr(.div, .{ .addr = .{ .regs = .@"bp+di", .off = 1000 } }, .none, true, try nextInstr(reader));

    try testExpectModSpecialInstr(.idiv, .{ .reg = .ax }, .none, true, try nextInstr(reader));
    try testExpectModSpecialInstr(.idiv, .{ .reg = .si }, .none, true, try nextInstr(reader));
    try testExpectModSpecialInstr(.idiv, .{ .addr = .{ .regs = .@"bp+si" } }, .none, false, try nextInstr(reader));
    try testExpectModSpecialInstr(.idiv, .{ .addr = .{ .regs = .bx, .off = 493 } }, .none, true, try nextInstr(reader));

    try testing.expectEqual(Instr{ .op = .aad, .payload = .none }, (try nextInstr(reader)).?);
    try testing.expectEqual(Instr{ .op = .cbw, .payload = .none }, (try nextInstr(reader)).?);
    try testing.expectEqual(Instr{ .op = .cwd, .payload = .none }, (try nextInstr(reader)).?);

    try testExpectModSpecialInstr(.not, .{ .reg = .ah }, .none, false, try nextInstr(reader));
    try testExpectModSpecialInstr(.not, .{ .reg = .bl }, .none, false, try nextInstr(reader));
    try testExpectModSpecialInstr(.not, .{ .reg = .sp }, .none, true, try nextInstr(reader));
    try testExpectModSpecialInstr(.not, .{ .reg = .si }, .none, true, try nextInstr(reader));
    try testExpectModSpecialInstr(.not, .{ .addr = .{ .regs = .bp } }, .none, true, try nextInstr(reader));
    try testExpectModSpecialInstr(.not, .{ .addr = .{ .regs = .bp, .off = 9905 } }, .none, false, try nextInstr(reader));

    for ([_]ModSpecialInstr.SrcOperand{ .{ .uimm = 1 }, .cl }) |src| {
        try testExpectModSpecialInstr(.sal, .{ .reg = .ah }, src, false, try nextInstr(reader));
        try testExpectModSpecialInstr(.shr, .{ .reg = .ax }, src, true, try nextInstr(reader));
        try testExpectModSpecialInstr(.sar, .{ .reg = .bx }, src, true, try nextInstr(reader));
        try testExpectModSpecialInstr(.rol, .{ .reg = .cx }, src, true, try nextInstr(reader));
        try testExpectModSpecialInstr(.ror, .{ .reg = .dh }, src, false, try nextInstr(reader));
        try testExpectModSpecialInstr(.rcl, .{ .reg = .sp }, src, true, try nextInstr(reader));
        try testExpectModSpecialInstr(.rcr, .{ .reg = .bp }, src, true, try nextInstr(reader));

        try testExpectModSpecialInstr(.sal, .{ .addr = .{ .regs = .bp, .off = 5 } }, src, true, try nextInstr(reader));
        try testExpectModSpecialInstr(
            .shr,
            .{ .addr = .{ .regs = .@"bx+si", .off = @bitCast(u16, @as(i16, -199)) } },
            src,
            src == .cl,
            try nextInstr(reader),
        );
        try testExpectModSpecialInstr(
            .sar,
            .{ .addr = .{ .regs = .@"bx+di", .off = @bitCast(u16, @as(i16, -300)) } },
            src,
            false,
            try nextInstr(reader),
        );
        try testExpectModSpecialInstr(.rol, .{ .addr = .{ .regs = .bp } }, src, src != .cl, try nextInstr(reader));
        try testExpectModSpecialInstr(.ror, .{ .addr = .{ .off = 4938 } }, src, src != .cl, try nextInstr(reader));
        try testExpectModSpecialInstr(.rcl, .{ .addr = .{ .off = 3 } }, src, false, try nextInstr(reader));
        try testExpectModSpecialInstr(.rcr, .{ .addr = .{ .regs = .bx } }, src, true, try nextInstr(reader));
    }

    const testAndOrXor = struct {
        fn f(r: anytype, op: Op) !void {
            try testExpectModInstr(op, .{ .reg = .al }, .{ .reg = .ah }, try nextInstr(r));
            try testExpectModInstr(op, .{ .reg = .ch }, .{ .reg = .cl }, try nextInstr(r));
            try testExpectModInstr(op, .{ .reg = .bp }, .{ .reg = .si }, try nextInstr(r));
            try testExpectModInstr(op, .{ .reg = .di }, .{ .reg = .sp }, try nextInstr(r));
            try testExpectDataInstr(op, .al, 93, try nextInstr(r));
            try testExpectDataInstr(op, .ax, 20392, try nextInstr(r));
            try testExpectModInstr(op, .{ .addr = .{ .regs = .@"bp+si", .off = 10 } }, .{ .reg = .ch }, try nextInstr(r));
            try testExpectModInstr(op, .{ .addr = .{ .regs = .@"bx+di", .off = 1000 } }, .{ .reg = .dx }, try nextInstr(r));
            try testExpectModInstr(op, .{ .reg = .bx }, .{ .addr = .{ .regs = .bp } }, try nextInstr(r));
            try testExpectModInstr(op, .{ .reg = .cx }, .{ .addr = .{ .off = 4384 } }, try nextInstr(r));
            try testExpectModSpecialInstr(
                op,
                .{ .addr = .{ .regs = .bp, .off = @bitCast(u8, @as(i8, -39)) } },
                .{ .uimm = 239 },
                false,
                try nextInstr(r),
            );
            try testExpectModSpecialInstr(
                op,
                .{ .addr = .{ .regs = .@"bx+si", .off = @bitCast(u16, @as(i16, -4332)) } },
                .{ .uimm = 10328 },
                true,
                try nextInstr(r),
            );
        }
    }.f;

    try testAndOrXor(reader, .@"and");

    try testCmpTest(reader, .@"test");

    try testAndOrXor(reader, .@"or");
    try testAndOrXor(reader, .xor);

    try testing.expectEqual(Instr{ .op = .movsb, .payload = .none, .exclusive_prefix = .rep }, (try nextInstr(reader)).?);
    try testing.expectEqual(Instr{ .op = .cmpsb, .payload = .none, .exclusive_prefix = .rep }, (try nextInstr(reader)).?);
    try testing.expectEqual(Instr{ .op = .scasb, .payload = .none, .exclusive_prefix = .rep }, (try nextInstr(reader)).?);
    try testing.expectEqual(Instr{ .op = .lodsb, .payload = .none, .exclusive_prefix = .rep }, (try nextInstr(reader)).?);
    try testing.expectEqual(Instr{ .op = .movsw, .payload = .none, .exclusive_prefix = .rep }, (try nextInstr(reader)).?);
    try testing.expectEqual(Instr{ .op = .cmpsw, .payload = .none, .exclusive_prefix = .rep }, (try nextInstr(reader)).?);
    try testing.expectEqual(Instr{ .op = .scasw, .payload = .none, .exclusive_prefix = .rep }, (try nextInstr(reader)).?);
    try testing.expectEqual(Instr{ .op = .lodsw, .payload = .none, .exclusive_prefix = .rep }, (try nextInstr(reader)).?);

    try testing.expectEqual(Instr{ .op = .stosb, .payload = .none, .exclusive_prefix = .rep }, (try nextInstr(reader)).?);
    try testing.expectEqual(Instr{ .op = .stosw, .payload = .none, .exclusive_prefix = .rep }, (try nextInstr(reader)).?);

    try testExpectModSpecialInstr(.call, .{ .addr = .{ .off = 39201 } }, .none, true, try nextInstr(reader));
    try testExpectModSpecialInstr(
        .call,
        .{ .addr = .{ .regs = .bp, .off = @bitCast(u8, @as(i8, -100)) } },
        .none,
        true,
        try nextInstr(reader),
    );
    try testExpectModSpecialInstr(.call, .{ .reg = .sp }, .none, true, try nextInstr(reader));
    try testExpectModSpecialInstr(.call, .{ .reg = .ax }, .none, true, try nextInstr(reader));

    try testExpectModSpecialInstr(.jmp, .{ .reg = .ax }, .none, true, try nextInstr(reader));
    try testExpectModSpecialInstr(.jmp, .{ .reg = .di }, .none, true, try nextInstr(reader));
    try testExpectModSpecialInstr(.jmp, .{ .addr = .{ .off = 12 } }, .none, true, try nextInstr(reader));
    try testExpectModSpecialInstr(.jmp, .{ .addr = .{ .off = 4395 } }, .none, true, try nextInstr(reader));

    try testing.expectEqual(
        Instr{ .op = .ret, .payload = .{ .imm = @bitCast(u16, @as(i16, -7)) } },
        (try nextInstr(reader)).?,
    );
    try testing.expectEqual(Instr{ .op = .ret, .payload = .{ .imm = 500 } }, (try nextInstr(reader)).?);
    try testing.expectEqual(Instr{ .op = .ret, .payload = .none }, (try nextInstr(reader)).?);

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
            Instr{ .op = op, .payload = .{ .ip_off = -2 * @intCast(i16, off) } },
            (try nextInstr(reader)).?,
        );
    }

    try testing.expectEqual(Instr{ .op = .int, .payload = .{ .imm = 13 } }, (try nextInstr(reader)).?);
    try testing.expectEqual(Instr{ .op = .int, .payload = .{ .imm = 3 } }, (try nextInstr(reader)).?);

    try testing.expectEqual(Instr{ .op = .into, .payload = .none }, (try nextInstr(reader)).?);
    try testing.expectEqual(Instr{ .op = .iret, .payload = .none }, (try nextInstr(reader)).?);

    try testing.expectEqual(Instr{ .op = .clc, .payload = .none }, (try nextInstr(reader)).?);
    try testing.expectEqual(Instr{ .op = .cmc, .payload = .none }, (try nextInstr(reader)).?);
    try testing.expectEqual(Instr{ .op = .stc, .payload = .none }, (try nextInstr(reader)).?);
    try testing.expectEqual(Instr{ .op = .cld, .payload = .none }, (try nextInstr(reader)).?);
    try testing.expectEqual(Instr{ .op = .std, .payload = .none }, (try nextInstr(reader)).?);
    try testing.expectEqual(Instr{ .op = .cli, .payload = .none }, (try nextInstr(reader)).?);
    try testing.expectEqual(Instr{ .op = .sti, .payload = .none }, (try nextInstr(reader)).?);
    try testing.expectEqual(Instr{ .op = .hlt, .payload = .none }, (try nextInstr(reader)).?);
    try testing.expectEqual(Instr{ .op = .wait, .payload = .none }, (try nextInstr(reader)).?);

    try testExpectPrefixedModSpecialInstr(
        .not,
        .{ .addr = .{ .regs = .bp, .off = 9905 } },
        .none,
        false,
        .lock,
        .none,
        try nextInstr(reader),
    );
    try testExpectPrefixedModInstr(
        .xchg,
        .{ .reg = .al },
        .{ .addr = .{ .off = 100 } },
        .lock,
        .none,
        try nextInstr(reader),
    );

    try testExpectPrefixedModInstr(
        .mov,
        .{ .reg = .al },
        .{ .addr = .{ .regs = .@"bx+si" } },
        .none,
        .@"cs:",
        try nextInstr(reader),
    );
    try testExpectPrefixedModInstr(
        .mov,
        .{ .reg = .bx },
        .{ .addr = .{ .regs = .@"bp+di" } },
        .none,
        .@"ds:",
        try nextInstr(reader),
    );
    try testExpectPrefixedModInstr(
        .mov,
        .{ .reg = .dx },
        .{ .addr = .{ .regs = .bp } },
        .none,
        .@"es:",
        try nextInstr(reader),
    );
    try testExpectPrefixedModInstr(
        .mov,
        .{ .reg = .ah },
        .{ .addr = .{ .regs = .@"bx+si", .off = 4 } },
        .none,
        .@"ss:",
        try nextInstr(reader),
    );

    try testExpectPrefixedModInstr(
        .@"and",
        .{ .addr = .{ .regs = .@"bp+si", .off = 10 } },
        .{ .reg = .ch },
        .none,
        .@"ss:",
        try nextInstr(reader),
    );
    try testExpectPrefixedModInstr(
        .@"or",
        .{ .addr = .{ .regs = .@"bx+di", .off = 1000 } },
        .{ .reg = .dx },
        .none,
        .@"ds:",
        try nextInstr(reader),
    );
    try testExpectPrefixedModInstr(
        .xor,
        .{ .reg = .bx },
        .{ .addr = .{ .regs = .bp } },
        .none,
        .@"es:",
        try nextInstr(reader),
    );
    try testExpectPrefixedModInstr(
        .cmp,
        .{ .reg = .cx },
        .{ .addr = .{ .off = 4384 } },
        .none,
        .@"es:",
        try nextInstr(reader),
    );
    try testExpectPrefixedModSpecialInstr(
        .@"test",
        .{ .addr = .{ .regs = .bp, .off = @bitCast(u8, @as(i8, -39)) } },
        .{ .uimm = 239 },
        false,
        .none,
        .@"cs:",
        try nextInstr(reader),
    );
    try testExpectPrefixedModSpecialInstr(
        .sbb,
        .{ .addr = .{ .regs = .@"bx+si", .off = @bitCast(u16, @as(i16, -4332)) } },
        .{ .uimm = 10328 },
        true,
        .none,
        .@"cs:",
        try nextInstr(reader),
    );

    try testExpectPrefixedModSpecialInstr(
        .not,
        .{ .addr = .{ .regs = .bp, .off = 9905 } },
        .none,
        false,
        .lock,
        .@"cs:",
        try nextInstr(reader),
    );

    try testing.expectEqual(
        Instr{ .op = .call, .payload = .{ .interseg_addr = .{ .cs = 123, .addr = 456 } } },
        (try nextInstr(reader)).?,
    );
    try testing.expectEqual(
        Instr{ .op = .jmp, .payload = .{ .interseg_addr = .{ .cs = 789, .addr = 34 } } },
        (try nextInstr(reader)).?,
    );

    try testExpectModInstr(.mov, .{ .addr = .{ .regs = .@"bx+si", .off = 59 } }, .{ .reg = .es }, try nextInstr(reader));

    try testing.expectEqual(Instr{ .op = .jmp, .payload = .{ .ip_off = 1753 } }, (try nextInstr(reader)).?);
    try testing.expectEqual(Instr{ .op = .call, .payload = .{ .ip_off = 10934 } }, (try nextInstr(reader)).?);

    try testing.expectEqual(Instr{ .op = .retf, .payload = .{ .imm = 17556 } }, (try nextInstr(reader)).?);
    try testing.expectEqual(Instr{ .op = .ret, .payload = .{ .imm = 17560 } }, (try nextInstr(reader)).?);
    try testing.expectEqual(Instr{ .op = .retf, .payload = .none }, (try nextInstr(reader)).?);
    try testing.expectEqual(Instr{ .op = .ret, .payload = .none }, (try nextInstr(reader)).?);

    inline for (.{ .call, .callf }) |op| {
        try testExpectModSpecialInstr(
            op,
            .{ .addr = .{ .regs = .@"bp+si", .off = @bitCast(u8, @as(i8, -0x3a)) } },
            .none,
            true,
            try nextInstr(reader),
        );
    }
    inline for (.{ .jmp, .jmpf }) |op| {
        try testExpectModSpecialInstr(
            op,
            .{ .addr = .{ .regs = .di } },
            .none,
            true,
            try nextInstr(reader),
        );
    }

    try testing.expectEqual(
        Instr{ .op = .jmp, .payload = .{ .interseg_addr = .{ .cs = 21862, .addr = 30600 } } },
        (try nextInstr(reader)).?,
    );

    try testing.expectEqual(@as(?Instr, null), try nextInstr(reader));
}

fn testExpectModInstr(op: Op, dst: ModOperand, src: ModOperand, instr: ?Instr) !void {
    return testExpectPrefixedModInstr(op, dst, src, .none, .none, instr);
}

fn testExpectPrefixedModInstr(
    op: Op,
    dst: ModOperand,
    src: ModOperand,
    exclusive_prefix: Instr.ExclusivePrefix,
    seg_override_prefix: Instr.SegOverridePrefix,
    instr: ?Instr,
) !void {
    try testing.expectEqual(op, instr.?.op);
    try testing.expectEqualDeep(dst, instr.?.payload.mod.getDst());
    try testing.expectEqualDeep(src, instr.?.payload.mod.getSrc());
    try testing.expectEqual(exclusive_prefix, instr.?.exclusive_prefix);
    try testing.expectEqual(seg_override_prefix, instr.?.seg_override_prefix);
}

fn testExpectModSpecialInstr(op: Op, dst: ModOperand, src: ModSpecialInstr.SrcOperand, w: bool, instr: ?Instr) !void {
    return testExpectPrefixedModSpecialInstr(op, dst, src, w, .none, .none, instr);
}

fn testExpectPrefixedModSpecialInstr(
    op: Op,
    dst: ModOperand,
    src: ModSpecialInstr.SrcOperand,
    w: bool,
    exclusive_prefix: Instr.ExclusivePrefix,
    seg_override_prefix: Instr.SegOverridePrefix,
    instr: ?Instr,
) !void {
    try testing.expectEqual(op, instr.?.op);
    try testing.expectEqualDeep(ModSpecialInstr{ .dst = dst, .src = src, .w = w }, instr.?.payload.mod_special);
    try testing.expectEqual(exclusive_prefix, instr.?.exclusive_prefix);
    try testing.expectEqual(seg_override_prefix, instr.?.seg_override_prefix);
}

fn testExpectDataInstr(op: Op, dst_reg: Register, imm: u16, instr: ?Instr) !void {
    try testing.expectEqual(op, instr.?.op);
    try testing.expectEqualDeep(DataInstr{ .dst_reg = dst_reg, .imm = imm }, instr.?.payload.data);
}

fn testExpectAddrInstr(op: Op, dst: AddrInstr.Operand, src: AddrInstr.Operand, instr: ?Instr) !void {
    try testing.expectEqual(op, instr.?.op);
    try testing.expectEqualDeep(dst, instr.?.payload.addr.getDst());
    try testing.expectEqualDeep(src, instr.?.payload.addr.getSrc());
}
