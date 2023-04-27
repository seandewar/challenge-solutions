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
cs: u16 = 0,
ds: u16 = 0,
es: u16 = 0,
ss: u16 = 0,

mem: [0x10000]u8 = .{0} ** 0x10000,

pub const Change = union(enum) {
    none,
    reg: struct { reg: decode.Register, old_val: u16 },
    memb: struct { addr: u16, old_val: u8 },
    memw: struct { addr: u16, old_val: u16 },
};

pub fn executeInstr(self: *Sim, instr: decode.Instr) !Change {
    const operands: struct { dst: Operand = .none, src: Operand = .none } = switch (instr.payload) {
        .mod => |mod| .{
            .dst = Operand.fromMod(mod.getDst(), mod.w),
            .src = Operand.fromMod(mod.getSrc(), mod.w),
        },

        .mod_special => |mod_special| .{
            .dst = Operand.fromMod(mod_special.dst, mod_special.w),
            .src = switch (mod_special.src) {
                .uimm => |uimm| .{ .imm = uimm },
                .simm => |simm| .{ .imm = if (mod_special.w)
                    @bitCast(u16, @as(i16, simm))
                else
                    @bitCast(u8, simm) },
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

        .ip_off => |_| return error.UnsupportedInstr, // TODO
        .interseg_addr => |_| return error.UnsupportedInstr, // TODO
        .none => .{},
    };

    return switch (instr.op) {
        .mov => self.writeOperand(operands.dst, self.readOperand(operands.src)),
        else => error.UnsupportedInstr,
    };
}

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
    return if (w) std.mem.readIntSliceLittle(u16, self.mem[addr..]) else self.mem[addr];
}

fn writeMemory(self: *Sim, addr: u16, w: bool, val: u16) Change {
    if (w) {
        const old_val = std.mem.readIntSliceLittle(u16, self.mem[addr..]);
        std.mem.writeIntSliceLittle(u16, self.mem[addr..], val);
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

// Following tests are based on the "Computer, Enhance!" perfaware listings:
// https://github.com/cmuratori/computer_enhance/blob/main/perfaware/part1
const testOpenListing = @import("test.zig").testOpenListing;

test "0043: immediate movs" {
    var file = try testOpenListing("0043_immediate_movs");
    defer file.close();

    var sim = Sim{};
    while (try decode.nextInstr(file.reader())) |instr| _ = try sim.executeInstr(instr);
    try testing.expectEqual(Sim{ .ax = 1, .bx = 2, .cx = 3, .dx = 4, .sp = 5, .bp = 6, .si = 7, .di = 8 }, sim);
}

test "0044: register movs" {
    var file = try testOpenListing("0044_register_movs");
    defer file.close();

    var sim = Sim{};
    while (try decode.nextInstr(file.reader())) |instr| _ = try sim.executeInstr(instr);
    try testing.expectEqual(Sim{ .ax = 4, .bx = 3, .cx = 2, .dx = 1, .sp = 1, .bp = 2, .si = 3, .di = 4 }, sim);
}
