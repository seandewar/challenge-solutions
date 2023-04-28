const std = @import("std");
const decode = @import("decode.zig");

pub fn writeInstr(instr: decode.Instr, writer: anytype) !void {
    if (instr.exclusive_prefix != .none) try writer.print("{s} ", .{@tagName(instr.exclusive_prefix)});
    try writer.writeAll(@tagName(instr.op));

    switch (instr.payload) {
        .mod => |mod| {
            try printModOperand(writer, mod.getDst(), instr.seg_override_prefix);
            try writer.writeByte(',');
            try printModOperand(writer, mod.getSrc(), instr.seg_override_prefix);
        },

        .mod_special => |mod_special| {
            if (mod_special.dst == .addr) {
                switch (instr.op) {
                    .call, .callf, .jmp, .jmpf => {}, // Unambiguously a wide ptr.
                    else => try writer.print(" {s} PTR", .{if (mod_special.w) "WORD" else "BYTE"}),
                }
            }
            try printModOperand(writer, mod_special.dst, instr.seg_override_prefix);
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
                .addr => |a| try writer.print(
                    " {s}[0x{x}]",
                    .{ if (instr.seg_override_prefix != .none) @tagName(instr.seg_override_prefix) else "", a },
                ),
            }
            if (i == 0) try writer.writeByte(',');
        },

        .data => |data| try writer.print(" {s}, 0x{x}", .{ @tagName(data.dst_reg), data.imm }),
        .ip_off => |ip_off| try writer.print(" ${s}0x{x}", .{ if (ip_off >= 0) "+" else "-", std.math.absCast(ip_off) }),
        .interseg_addr => |interseg_addr| try writer.print(" 0x{x}:0x{x}", .{ interseg_addr.cs, interseg_addr.addr }),
        .regs => |regs| try writer.print(" {s}, {s}", .{ @tagName(regs.dst), @tagName(regs.src) }),
        .reg => |reg| try writer.print(" {s}", .{@tagName(reg)}),
        .imm => |imm| try writer.print(" 0x{x}", .{imm}),
        .none => {},
    }
}

fn printModOperand(
    writer: anytype,
    operand: decode.ModOperand,
    seg_override_prefix: decode.Instr.SegOverridePrefix,
) !void {
    switch (operand) {
        .reg => |reg| try writer.print(" {s}", .{@tagName(reg)}),
        .addr => |ea| {
            try writer.print(" {s}[{s}", .{
                if (seg_override_prefix != .none) @tagName(seg_override_prefix) else "",
                if (ea.regs != .none) @tagName(ea.regs) else "",
            });
            if (ea.off != 0) try writer.print("{s}0x{x}", .{ if (ea.regs != .none) "+" else "", ea.off });
            try writer.writeByte(']');
        },
    }
}
