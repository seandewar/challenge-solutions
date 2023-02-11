const std = @import("std");

pub fn isPangram(str: []const u8) bool {
    var set: u26 = 0; // std.StaticBitSet also works.
    for (str) |c| {
        if (!std.ascii.isAlphabetic(c)) continue;
        set |= @as(u26, 1) << @intCast(u5, std.ascii.toLower(c) - 'a');
        if (set == std.math.maxInt(u26)) return true;
    }
    return false;
}
