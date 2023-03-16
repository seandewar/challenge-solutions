const std = @import("std");

pub fn abbreviate(allocator: std.mem.Allocator, words: []const u8) std.mem.Allocator.Error![]u8 {
    var out = std.ArrayList(u8).init(allocator);
    errdefer out.deinit();
    var it = std.mem.tokenize(u8, words, " -");
    while (it.next()) |word| for (word) |c| {
        if (!std.ascii.isAlphabetic(c)) continue;
        try out.append(std.ascii.toUpper(c));
        break;
    };
    return out.toOwnedSlice();
}
