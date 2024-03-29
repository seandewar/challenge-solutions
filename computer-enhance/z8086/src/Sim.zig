const std = @import("std");
const testing = std.testing;
const decode = @import("decode.zig");
const Sim = @This();

ax: u16 = 0,
bx: u16 = 0,
cx: u16 = 0,
dx: u16 = 0,
sp: u16 = 0,
bp: u16 = 0,
si: u16 = 0,
di: u16 = 0,
es: u16 = 0,
ss: u16 = 0,
ds: u16 = 0,
cs: u16 = 0,
ip: u16 = 0,

flags: Flags = .{},

mem: *Memory,

pub const Flag = enum { c, p, a, z, s, o, i, d, t };
pub const Flags = std.EnumSet(Flag);

pub const Memory = [0x10000]u8;

pub const Change = union(enum) {
    none,
    reg: struct { reg: decode.Register, old_val: u16 },
    memb: struct { addr: u16, old_val: u8 },
    memw: struct { addr: u16, old_val: u16 },
    loop_old_ip: u16,
};

pub fn step(self: *Sim) !struct { instr: ?decode.Instr, change: Change } {
    var stream = std.io.FixedBufferStream(*Memory){ .buffer = self.mem, .pos = self.ip };
    var it = decode.instrIterator(stream.reader());
    const instr = (try it.next()) orelse return .{ .instr = null, .change = .none };
    return .{
        .instr = instr,
        .change = try self.executeInstr(instr, @intCast(u16, it.getPrevBytes().len)),
    };
}

fn executeInstr(self: *Sim, instr: decode.Instr, instr_size: u16) !Change {
    const operands: Operands = switch (instr.payload) {
        .mod => |mod| .{
            .dst = Operand.fromMod(mod.getDst(), mod.w),
            .src = Operand.fromMod(mod.getSrc(), mod.w),
        },

        .mod_special => |mod_special| .{
            .dst = Operand.fromMod(mod_special.dst, mod_special.w),
            .src = switch (mod_special.src) {
                .uimm => |uimm| .{ .imm = uimm },
                .simm => |simm| .{ .imm = if (mod_special.w) @bitCast(u16, @as(i16, simm)) else @bitCast(u8, simm) },
                .cl => .{ .reg = .cl },
                .none => .none,
            },
        },

        .addr => |addr| .{
            .dst = Operand.fromAddr(addr.getDst(), addr.w),
            .src = Operand.fromAddr(addr.getSrc(), addr.w),
        },

        .data => |data| .{ .dst = .{ .reg = data.dst_reg }, .src = .{ .imm = data.imm } },
        .regs => |regs| .{ .dst = .{ .reg = regs.dst }, .src = .{ .reg = regs.src } },
        .reg => |reg| .{ .dst = .{ .reg = reg } },
        .imm => |imm| .{ .src = .{ .imm = imm } },
        .ip_off => |ip_off| .{ .src = .{ .ip_off = ip_off.off } },

        .interseg_addr => return error.UnsupportedInstr, // TODO
        .none => .{},
    };

    const change = switch (instr.op) {
        .mov => self.writeOperand(operands.dst, self.readOperand(operands.src)),

        .add, .sub, .cmp => blk: {
            const a = self.readOperand(operands.dst);
            const b = self.readOperand(operands.src);
            const result: u16 = if (operands.isDstWide())
                self.doAddSub(true, instr.op == .add, a, b)
            else
                self.doAddSub(false, instr.op == .add, @intCast(u8, a), @intCast(u8, b));

            break :blk if (instr.op != .cmp) self.writeOperand(operands.dst, result) else .none;
        },

        .jb => self.doJump(self.flags.contains(.c), operands.src.ip_off, false),
        .jbe => self.doJump(self.flags.contains(.c) or self.flags.contains(.z), operands.src.ip_off, false),
        .jcxz => self.doJump(self.cx == 0, operands.src.ip_off, false),
        .je => self.doJump(self.flags.contains(.z), operands.src.ip_off, false),
        .jl => self.doJump(self.flags.contains(.s) != self.flags.contains(.o), operands.src.ip_off, false),
        .jle => self.doJump((self.flags.contains(.s) != self.flags.contains(.o)) or self.flags.contains(.z), operands.src.ip_off, false),
        .jmp => self.doJump(true, operands.src.ip_off, false),
        .jnb => self.doJump(!self.flags.contains(.c), operands.src.ip_off, false),
        .jnbe => self.doJump(!self.flags.contains(.c) and !self.flags.contains(.z), operands.src.ip_off, false),
        .jne => self.doJump(!self.flags.contains(.z), operands.src.ip_off, false),
        .jnl => self.doJump(self.flags.contains(.s) == self.flags.contains(.o), operands.src.ip_off, false),
        .jnle => self.doJump((self.flags.contains(.s) == self.flags.contains(.o)) and !self.flags.contains(.z), operands.src.ip_off, false),
        .jno => self.doJump(!self.flags.contains(.o), operands.src.ip_off, false),
        .jnp => self.doJump(!self.flags.contains(.p), operands.src.ip_off, false),
        .jns => self.doJump(!self.flags.contains(.s), operands.src.ip_off, false),
        .jo => self.doJump(self.flags.contains(.o), operands.src.ip_off, false),
        .jp => self.doJump(self.flags.contains(.p), operands.src.ip_off, false),
        .js => self.doJump(self.flags.contains(.s), operands.src.ip_off, false),
        .loop => self.doJump(true, operands.src.ip_off, true),
        .loope => self.doJump(self.flags.contains(.z), operands.src.ip_off, true),
        .loopne => self.doJump(!self.flags.contains(.z), operands.src.ip_off, true),

        else => return error.UnsupportedInstr,
    };

    self.ip +%= instr_size;
    return change;
}

fn doAddSub(self: *Sim, comptime w: bool, add: bool, a: if (w) u16 else u8, b: @TypeOf(a)) @TypeOf(a) {
    const result = if (add) @addWithOverflow(a, b) else @subWithOverflow(a, b);
    self.flags.setPresent(.c, result[1] == 1);
    self.flags.setPresent(.z, result[0] == 0);
    self.flags.setPresent(.p, @popCount(result[0]) % 2 == 0);

    const Signed = if (w) i16 else i8;
    self.flags.setPresent(.s, @bitCast(Signed, result[0]) < 0);

    const as = @bitCast(Signed, a);
    const bs = @bitCast(Signed, b);
    self.flags.setPresent(.o, (if (add) @addWithOverflow(as, bs) else @subWithOverflow(as, bs))[1] == 1);

    const an = @truncate(u4, a);
    const bn = @truncate(u4, b);
    self.flags.setPresent(.a, (if (add) @addWithOverflow(an, bn) else @subWithOverflow(an, bn))[1] == 1);

    return result[0];
}

inline fn doJump(self: *Sim, cond: bool, ip_off: i16, comptime is_loop: bool) Change {
    if (is_loop) self.cx -%= 1;
    const should_jump = cond and (!is_loop or self.cx != 0);
    const change = if (should_jump) self.writeRegister(.ip, self.ip +% @bitCast(u16, ip_off)) else .none;
    return if (is_loop and change != .none) .{ .loop_old_ip = change.reg.old_val } else change;
}

const Operands = struct {
    dst: Operand = .none,
    src: Operand = .none,

    fn isDstWide(self: Operands) bool {
        return switch (self.dst) {
            .reg => |reg| reg.isWide(),
            .effective_addr => |effective_addr| effective_addr.w,
            .addr_reg => |addr_reg| addr_reg.w,
            else => unreachable,
        };
    }
};

const Operand = union(enum) {
    none,
    reg: decode.Register,
    imm: u16,
    effective_addr: struct { ea: decode.EffectiveAddr, w: bool },
    addr_reg: struct { reg: decode.Register, w: bool },
    ip_off: i16,

    fn fromMod(operand: decode.ModOperand, w: bool) Operand {
        return switch (operand) {
            .reg => |reg| .{ .reg = reg },
            .addr => |ea| .{ .effective_addr = .{ .ea = ea, .w = w } },
        };
    }

    fn fromAddr(operand: decode.AddrInstr.Operand, w: bool) Operand {
        return switch (operand) {
            .reg => |reg| .{ .reg = reg },
            .addr => |addr| .{ .effective_addr = .{
                .ea = .{ .regs = .none, .off = addr },
                .w = w,
            } },
        };
    }
};

fn calcEffectiveAddr(self: Sim, ea: decode.EffectiveAddr) u16 {
    return switch (ea.regs) {
        .none => 0,
        .@"bx+si" => self.bx + self.si,
        .@"bx+di" => self.bx + self.di,
        .@"bp+si" => self.bp + self.si,
        .@"bp+di" => self.bp + self.di,
        inline else => |tag| @field(self, @tagName(tag)),
    } +% ea.off;
}

fn readOperand(self: Sim, operand: Operand) u16 {
    return switch (operand) {
        .reg => |reg| self.readRegister(reg),
        .imm => |imm| imm,
        .effective_addr => |effective_addr| self.readMemory(
            self.calcEffectiveAddr(effective_addr.ea),
            effective_addr.w,
        ),
        .addr_reg => |addr_reg| self.readMemory(
            self.readRegister(addr_reg.reg),
            addr_reg.w,
        ),
        .none, .ip_off => unreachable,
    };
}

fn writeOperand(self: *Sim, operand: Operand, val: u16) Change {
    return switch (operand) {
        .reg => |reg| self.writeRegister(reg, val),
        .effective_addr => |effective_addr| self.writeMemory(
            self.calcEffectiveAddr(effective_addr.ea),
            effective_addr.w,
            val,
        ),
        .addr_reg => |addr_reg| self.writeMemory(
            self.readRegister(addr_reg.reg),
            addr_reg.w,
            val,
        ),
        .none, .imm, .ip_off => unreachable,
    };
}

pub inline fn readMemory(self: Sim, addr: u16, w: bool) u16 {
    return if (w)
        std.mem.readIntLittle(u16, &.{ self.mem[addr], self.mem[addr +% 1] })
    else
        self.mem[addr];
}

fn writeMemory(self: *Sim, addr: u16, w: bool, val: u16) Change {
    if (w) {
        const old_val = std.mem.readIntLittle(u16, &.{ self.mem[addr], self.mem[addr +% 1] });
        self.mem[addr] = @truncate(u8, val);
        self.mem[addr +% 1] = @truncate(u8, val >> 8);
        return .{ .memw = .{ .addr = addr, .old_val = old_val } };
    } else {
        const old_val = self.mem[addr];
        self.mem[addr] = @intCast(u8, val);
        return .{ .memb = .{ .addr = addr, .old_val = old_val } };
    }
}

pub fn readRegister(self: Sim, reg: decode.Register) u16 {
    return switch (reg) {
        inline .al, .bl, .cl, .dl => |tag| @truncate(u8, @field(self, @tagName(tag)[0..1] ++ "x")),
        inline .ah, .bh, .ch, .dh => |tag| @truncate(u8, @field(self, @tagName(tag)[0..1] ++ "x") >> 8),
        inline else => |tag| @field(self, @tagName(tag)),
    };
}

fn writeRegister(self: *Sim, reg: decode.Register, val: u16) Change {
    const change: Change = switch (reg) {
        .al, .ah, .ax => .{ .reg = .{ .reg = .ax, .old_val = self.ax } },
        .bl, .bh, .bx => .{ .reg = .{ .reg = .bx, .old_val = self.bx } },
        .cl, .ch, .cx => .{ .reg = .{ .reg = .cx, .old_val = self.cx } },
        .dl, .dh, .dx => .{ .reg = .{ .reg = .dx, .old_val = self.dx } },
        inline else => |tag| .{ .reg = .{ .reg = tag, .old_val = @field(self, @tagName(tag)) } },
    };

    switch (reg) {
        inline .al, .bl, .cl, .dl => |tag| {
            const wreg = &@field(self, @tagName(tag)[0..1] ++ "x");
            wreg.* = (wreg.* & ~@as(u16, 0xff)) | @intCast(u8, val);
        },
        inline .ah, .bh, .ch, .dh => |tag| {
            const wreg = &@field(self, @tagName(tag)[0..1] ++ "x");
            wreg.* = (wreg.* & 0xff) | (@as(u16, @intCast(u8, val)) << 8);
        },
        inline else => |tag| @field(self, @tagName(tag)) = val,
    }

    return change;
}

test "wrapping read/write works" {
    var mem = std.mem.zeroes(Sim.Memory);
    var sim = Sim{ .mem = &mem };
    _ = sim.writeMemory(0xffff, true, 0x1234);
    try testing.expectEqual(@as(u16, 0x1234), sim.readMemory(0xffff, true));
}

// Following tests are based on the "Computer, Enhance!" perfaware listings:
// https://github.com/cmuratori/computer_enhance/blob/main/perfaware/part1
const testOpenListing = @import("test.zig").testOpenListing;

test "0043: immediate movs" {
    var mem: Memory = undefined;
    try testExpectSim(
        .{ .ax = 1, .bx = 2, .cx = 3, .dx = 4, .sp = 5, .bp = 6, .si = 7, .di = 8 },
        try testRunListing("0043_immediate_movs", &mem),
    );
}

test "0044: register movs" {
    var mem: Memory = undefined;
    try testExpectSim(
        .{ .ax = 4, .bx = 3, .cx = 2, .dx = 1, .sp = 1, .bp = 2, .si = 3, .di = 4 },
        try testRunListing("0044_register_movs", &mem),
    );
}

test "0045: challenge register movs" {
    var mem: Memory = undefined;
    try testExpectSim(
        .{
            .ax = 0x4411,
            .bx = 0x3344,
            .cx = 0x6677,
            .dx = 0x7788,
            .sp = 0x4411,
            .bp = 0x3344,
            .si = 0x6677,
            .di = 0x7788,
            .es = 0x6677,
            .ss = 0x4411,
            .ds = 0x3344,
        },
        try testRunListing("0045_challenge_register_movs", &mem),
    );
}

test "0046: add sub cmp" {
    var mem: Memory = undefined;
    try testExpectSim(
        .{
            .bx = 0xe102,
            .cx = 0x0f01,
            .sp = 0x03e6,
            .flags = &.{ .p, .z },
        },
        try testRunListing("0046_add_sub_cmp", &mem),
    );
}

test "0047: challenge flags" {
    var mem: Memory = undefined;
    try testExpectSim(
        .{
            .bx = 0x9ca5,
            .dx = 0x000a,
            .sp = 0x0063,
            .bp = 0x0062,
            .flags = &.{ .c, .p, .a, .s },
        },
        try testRunListing("0047_challenge_flags", &mem),
    );
}

test "0048: ip register" {
    var mem: Memory = undefined;
    try testExpectSim(
        .{
            .bx = 0x07d0,
            .cx = 0xfce0,
            .ip = 0x000e,
            .flags = &.{ .c, .s },
        },
        try testRunListing("0048_ip_register", &mem),
    );
}

test "0049: conditional jumps" {
    var mem: Memory = undefined;
    try testExpectSim(
        .{
            .bx = 0x0406,
            .ip = 0x000e,
            .flags = &.{ .p, .z },
        },
        try testRunListing("0049_conditional_jumps", &mem),
    );
}

test "0050: challenge jumps" {
    var mem: Memory = undefined;
    try testExpectSim(
        .{
            .ax = 0x000d,
            .bx = 0xfffb,
            .ip = 0x001c,
            .flags = &.{ .c, .a, .s },
        },
        try testRunListing("0050_challenge_jumps", &mem),
    );
}

test "0051: memory mov" {
    var mem: Memory = undefined;
    try testExpectSim(
        .{
            .bx = 0x0001,
            .cx = 0x0002,
            .dx = 0x000a,
            .bp = 0x0004,
            .ip = 0x0030,
        },
        try testRunListing("0051_memory_mov", &mem),
    );
}

test "0052: memory add loop" {
    var mem: Memory = undefined;
    try testExpectSim(
        .{
            .bx = 0x0006,
            .cx = 0x0004,
            .dx = 0x0006,
            .bp = 0x03e8,
            .si = 0x0006,
            .ip = 0x0023,
            .flags = &.{ .p, .z },
        },
        try testRunListing("0052_memory_add_loop", &mem),
    );
}

test "0053: add loop challenge" {
    var mem: Memory = undefined;
    try testExpectSim(
        .{
            .bx = 0x0006,
            .dx = 0x0006,
            .bp = 0x03e6,
            .ip = 0x0021,
            .flags = &.{ .p, .z },
        },
        try testRunListing("0053_add_loop_challenge", &mem),
    );
}

test "0054: draw rectangle" {
    var mem: Memory = undefined;
    try testExpectSim(
        .{
            .cx = 0x0040,
            .dx = 0x0040,
            .bp = 0x4100,
            .ip = 0x0026,
            .flags = &.{ .p, .z },
        },
        try testRunListing("0054_draw_rectangle", &mem),
    );
}

test "0055: challenge rectangle" {
    var mem: Memory = undefined;
    try testExpectSim(
        .{
            .bx = 0x4004,
            .bp = 0x02fc,
            .ip = 0x0044,
            .flags = &.{.p},
        },
        try testRunListing("0055_challenge_rectangle", &mem),
    );
}

pub fn testRunListing(name: []const u8, mem: *Memory) !Sim {
    const end_i = blk: {
        var file = try testOpenListing(name);
        defer file.close();

        const end_ip = try file.readAll(mem);
        @memset(mem[end_ip..], 0);
        break :blk end_ip;
    };

    var sim = Sim{ .mem = mem };
    while (sim.ip < end_i) _ = try sim.step();
    return sim;
}

const ExpectedSim = struct {
    ax: u16 = 0,
    bx: u16 = 0,
    cx: u16 = 0,
    dx: u16 = 0,
    sp: u16 = 0,
    bp: u16 = 0,
    si: u16 = 0,
    di: u16 = 0,
    es: u16 = 0,
    ss: u16 = 0,
    ds: u16 = 0,
    cs: u16 = 0,

    /// null = don't care.
    ip: ?u16 = null,

    flags: []const Flag = &.{},
};

fn testExpectSim(expected: ExpectedSim, actual: Sim) !void {
    inline for (@typeInfo(ExpectedSim).Struct.fields) |field| switch (field.type) {
        u16 => {
            try testing.expectEqual(@field(expected, field.name), @field(actual, field.name));
        },
        ?u16 => if (@field(expected, field.name)) |expected_val| {
            try testing.expectEqual(expected_val, @field(actual, field.name));
        },
        []const Flag => {
            try testing.expectEqual(Flags.initMany(expected.flags), @field(actual, field.name));
        },
        else => {},
    };
}
