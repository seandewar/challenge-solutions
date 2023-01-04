const std = @import("std");
const answer = blk: {
    @setEvalBranchQuota(10_000);
    const input = @embedFile("input");
    var sum = 0;
    var line_it = std.mem.tokenize(u8, input, std.cstr.line_sep);
    while (line_it.next()) |line| {
        var value = 0;
        var mul = 1;
        var digit_it = std.mem.reverseIterator(line);
        while (digit_it.next()) |c| : (mul *= 5)
            value += mul * (@intCast(i64, std.mem.indexOfScalar(u8, "=-012", c).?) - 2);
        sum += value;
    }
    if (sum == 0) break :blk "0";
    var result = std.BoundedArray(u8, 128).init(0) catch unreachable;
    while (sum != 0) : (sum = (sum + 2) / 5) result.append("012=-"[sum % 5]) catch unreachable;
    std.mem.reverse(u8, result.slice());
    break :blk result.slice();
};
pub fn main() !void {
    const stdout = std.io.getStdOut().writer();
    try stdout.print("Day25 (comptime only): {s}\n", .{answer});
}
