const std = @import("std");
const Allocator = std.mem.Allocator;

pub fn calculateHandshake(allocator: Allocator, x: isize) Allocator.Error![]const Signal {
    var buf = try std.ArrayListUnmanaged(Signal).initCapacity(allocator, @popCount(x & ~(@as(u5, 1) << 4)));
    inline for (std.meta.fields(Signal)) |signal|
        if (x & signal.value != 0) buf.appendAssumeCapacity(@field(Signal, signal.name));
    if (x & (1 << 4) != 0) std.mem.reverse(Signal, buf.items);
    return buf.items;
}

pub const Signal = enum(u4) {
    wink = 1,
    double_blink = 1 << 1,
    close_your_eyes = 1 << 2,
    jump = 1 << 3,
};
