const std = @import("std");

const top3 = blk: {
    @setEvalBranchQuota(100_000);
    const input = @embedFile("input");
    var result = [_]u32{0} ** 3;
    var line_it = std.mem.split(u8, input, std.cstr.line_sep);
    var accum: u32 = 0;
    while (line_it.next()) |line| {
        if (line.len == 0) {
            const i = std.mem.indexOfMin(u32, &result);
            result[i] = @max(result[i], accum);
            accum = 0;
        } else accum += std.fmt.parseInt(u32, line, 10) catch unreachable;
    }
    const i = std.mem.indexOfMin(u32, &result);
    result[i] = @max(result[i], accum);
    break :blk result;
};

const p1 = std.mem.max(u32, &top3);
const p2 = top3[0] + top3[1] + top3[2];

pub fn main() !void {
    const stdout = std.io.getStdOut().writer();
    try stdout.print("Day1 (comptime only): P1: {}, P2: {}\n", .{ p1, p2 });
}
