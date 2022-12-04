const std = @import("std");

const answers = blk: {
    @setEvalBranchQuota(1_000_000);
    const input = @embedFile("input");
    var line_it = std.mem.tokenize(u8, input, "\n");
    var p1: u32 = 0;
    var p2: u32 = 0;
    while (line_it.next()) |line| {
        var ranges_it = std.mem.split(u8, line, ",");
        var range_it = std.mem.split(u8, ranges_it.next().?, "-");
        const a = std.fmt.parseInt(u32, range_it.next().?, 10) catch unreachable;
        const b = std.fmt.parseInt(u32, range_it.next().?, 10) catch unreachable;
        range_it = std.mem.split(u8, ranges_it.next().?, "-");
        const x = std.fmt.parseInt(u32, range_it.next().?, 10) catch unreachable;
        const y = std.fmt.parseInt(u32, range_it.next().?, 10) catch unreachable;
        if ((a <= x and b >= y) or (x <= a and y >= b)) p1 += 1;
        if (a <= y and x <= b) p2 += 1;
    }
    break :blk .{ p1, p2 };
};

pub fn main() !void {
    const stdout = std.io.getStdOut().writer();
    try stdout.print("Day4 (comptime only): P1: {}, P2: {}\n", answers);
}
