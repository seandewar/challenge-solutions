const std = @import("std");
const Allocator = std.mem.Allocator;
const allocPrint = std.fmt.allocPrint;

pub fn recite(allocator: Allocator, words: []const []const u8) (std.fmt.AllocPrintError || Allocator.Error)![][]u8 {
    var result = try std.ArrayListUnmanaged([]u8).initCapacity(allocator, words.len);
    if (words.len == 0) return result.items;
    errdefer {
        for (result.items) |item| allocator.free(item);
        result.deinit(allocator);
    }

    for (words[0 .. words.len - 1]) |word, i|
        result.appendAssumeCapacity(try allocPrint(allocator, "For want of a {s} the {s} was lost.\n", .{ word, words[i + 1] }));

    result.appendAssumeCapacity(try allocPrint(allocator, "And all for the want of a {s}.\n", .{words[0]}));
    return result.items;
}
