const std = @import("std");

const tree_grid = blk: {
    @setEvalBranchQuota(100_000);
    const input = @embedFile("input");
    var grid: [99]*const [99]u8 = undefined;
    var line_i = 0;
    var line_it = std.mem.tokenize(u8, input, "\n");
    while (line_it.next()) |line| : (line_i += 1) grid[line_i] = line[0..];
    break :blk grid;
};

const p1 = blk: {
    @setEvalBranchQuota(1_000_000);
    var vis_grid = std.mem.zeroes([99][99]bool);
    var i = 0;
    while (i < 99) : (i += 1) {
        for (.{ .{ i, 0, 0, 1 }, .{ i, 98, 0, -1 }, .{ 0, i, 1, 0 }, .{ 98, i, -1, 0 } }) |dir| {
            var max_tree = 0;
            var x = dir[0];
            var y = dir[1];
            while (x >= 0 and x < 99 and y >= 0 and y < 99) : ({
                x += dir[2];
                y += dir[3];
            }) {
                if (tree_grid[y][x] > max_tree) vis_grid[y][x] = true;
                max_tree = @max(max_tree, tree_grid[y][x]);
            }
        }
    }
    var result = 0;
    for (vis_grid) |row| result += std.mem.count(bool, &row, &.{true});
    break :blk result;
};

const p2 = blk: {
    @setEvalBranchQuota(1_000_000);
    var result = 0;
    for (tree_grid) |row, iy| {
        for (row) |tree, ix| {
            var tree_score = 1;
            for (.{ .{ 1, 0 }, .{ -1, 0 }, .{ 0, 1 }, .{ 0, -1 } }) |dir| {
                var score = 0;
                var x = @intCast(i8, ix) + dir[0];
                var y = @intCast(i8, iy) + dir[1];
                while (x >= 0 and x < 99 and y >= 0 and y < 99) : ({
                    x += dir[0];
                    y += dir[1];
                }) {
                    score += 1;
                    if (tree <= tree_grid[y][x]) break;
                }
                tree_score *= score;
            }
            result = @max(result, tree_score);
        }
    }
    break :blk result;
};

pub fn main() !void {
    const stdout = std.io.getStdOut().writer();
    try stdout.print("Day8 (comptime only): P1: {}, P2: {}\n", .{ p1, p2 });
}
