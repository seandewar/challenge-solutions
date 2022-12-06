const std = @import("std");

const input = @embedFile("input");
fn findMarker(len: usize) usize {
    var i: usize = 0;
    while (i < input.len - len) : (i += 1) {
        var seen = std.StaticBitSet(26).initEmpty();
        for (input[i .. i + len]) |c| seen.set(c - 'a');
        if (seen.count() == len) return i + len;
    }
    unreachable;
}
const answers = blk: {
    @setEvalBranchQuota(1_000_000);
    break :blk .{ findMarker(4), findMarker(14) };
};

pub fn main() !void {
    const stdout = std.io.getStdOut().writer();
    try stdout.print("Day6 (comptime only): P1: {}, P2: {}\n", answers);
}
