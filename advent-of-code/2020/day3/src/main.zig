///! Computed fully at compile time!
const std = @import("std");

const map = @embedFile("input.txt");
const width: u16 = std.mem.indexOfScalar(u8, map, '\n') orelse map.len;
const height: u16 = blk: {
    @setEvalBranchQuota(50_000);
    break :blk std.mem.count(u8, map, "\n");
};

fn checkSlope(dx: u16, dy: u16) u32 {
    var ret: u32 = 0;
    var x: u16 = dx;
    var y: u16 = dy;
    while (y < height) : (y += dy) {
        const square = map[y * (width + 1) + x];
        if (square == '#') {
            ret += 1;
        }
        x += dx;
        x %= width;
    }
    return ret;
}

pub fn main() !void {
    const day1_result = comptime checkSlope(3, 1);
    const day2_result = comptime blk: {
        @setEvalBranchQuota(10_000);
        break :blk @as(u64, day1_result) * checkSlope(1, 1) * checkSlope(5, 1) * checkSlope(7, 1) * checkSlope(1, 2);
    };

    const stdout = std.io.getStdOut().writer();
    try stdout.print("Day 3:\nP1: {d}\nP2: {d}\n", .{ day1_result, day2_result });
}
