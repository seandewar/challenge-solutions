const std = @import("std");

const elevations = blk: {
    @setEvalBranchQuota(100_000);
    const input = @embedFile("input");
    var line_it = std.mem.tokenize(u8, input, std.cstr.line_sep);
    var w: u8 = line_it.peek().?.len;
    var h: u8 = 0;
    while (line_it.next()) |_| h += 1;
    var grid: [h]*const [w]u8 = undefined;
    line_it.reset();
    for (grid) |*row| row.* = line_it.next().?[0..];
    break :blk .{ .grid = grid, .w = w, .h = h };
};

fn computeAnswer(comptime p1: bool) u16 {
    var dist_grid = [_][elevations.w]?u16{.{null} ** elevations.w} ** elevations.h;
    for (elevations.grid) |row, y| find_starts: {
        for (row) |elev, x| {
            if (elev == 'S' or (elev == 'a' and !p1)) {
                dist_grid[y][x] = 0;
                if (p1) break :find_starts;
            }
        }
    }
    var steps: u16 = 1;
    while (true) : (steps += 1) {
        for (dist_grid) |dist_row, y| {
            for (dist_row) |dist, x| {
                if (dist == null or dist.? != steps - 1) continue;
                const elev = if (elevations.grid[y][x] == 'S') 'a' else elevations.grid[y][x];
                for (.{ .{ 1, 0 }, .{ -1, 0 }, .{ 0, 1 }, .{ 0, -1 } }) |dir| {
                    const new_x = @intCast(i8, x) + dir[0];
                    const new_y = @intCast(i8, y) + dir[1];
                    if (new_x < 0 or new_y < 0 or new_x >= elevations.w or new_y >= elevations.h) continue;
                    if (elevations.grid[new_y][new_x] == 'E' and elev >= 'y') return steps;
                    if (dist_grid[new_y][new_x] != null or elevations.grid[new_y][new_x] > elev + 1) continue;
                    dist_grid[new_y][new_x] = steps;
                }
            }
        }
    }
}
const answers = blk: {
    @setEvalBranchQuota(10_000_000);
    break :blk .{ computeAnswer(true), computeAnswer(false) };
};

pub fn main() !void {
    const stdout = std.io.getStdOut().writer();
    try stdout.print("Day12 (comptime only): P1: {}, P2: {}\n", answers);
}
