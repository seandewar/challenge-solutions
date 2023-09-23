const std = @import("std");

pub fn isBalanced(allocator: std.mem.Allocator, s: []const u8) !bool {
    _ = allocator;
    var stack = std.BoundedArray(u8, 10){};
    for (s) |c| switch (c) {
        '(' => try stack.append(')'),
        '{' => try stack.append('}'),
        '[' => try stack.append(']'),
        ')', '}', ']' => if ((stack.popOrNull() orelse return false) != c) return false,
        else => {},
    };
    return stack.len == 0;
}
