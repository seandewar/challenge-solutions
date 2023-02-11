const ascii = @import("std").ascii;

pub fn isIsogram(str: []const u8) bool {
    var set: u26 = 0; // std.StaticBitSet also works.
    for (str) |c| {
        if (!ascii.isAlphabetic(c)) continue;
        const mask = @as(u26, 1) << @intCast(u5, ascii.toLower(c) - 'a');
        if (set & mask != 0) return false;
        set |= mask;
    }
    return true;
}
