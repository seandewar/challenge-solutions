const std = @import("std");

const Position = struct { x: u16, y: u16 };
fn pathParseNext(line: []const u8, it: *?std.mem.SplitIterator(u8)) ?Position {
    if (it.* == null) it.* = std.mem.split(u8, line, " -> ");
    const pos = it.*.?.next() orelse return null;
    const comma_i = std.mem.indexOfScalar(u8, pos, ',') orelse unreachable;
    return Position{
        .x = std.fmt.parseInt(u16, pos[0..comma_i], 10) catch unreachable,
        .y = std.fmt.parseInt(u16, pos[comma_i + 1 ..], 10) catch unreachable,
    };
}
const parsed = blk: {
    @setEvalBranchQuota(1_000_000);
    const input = @embedFile("input");
    var x_p1bound = [_]u16{ 500, 500 };
    var y_floor = 2;
    var line_it = std.mem.tokenize(u8, input, std.cstr.line_sep);
    while (line_it.next()) |line| {
        var pos_it: ?std.mem.SplitIterator(u8) = null;
        while (pathParseNext(line, &pos_it)) |pos| {
            x_p1bound = .{ @min(x_p1bound[0], pos.x), @max(x_p1bound[1], pos.x) };
            y_floor = @max(y_floor, pos.y + 2);
        }
    }
    const x_bound = [_]u16{ 501 - y_floor, 499 + y_floor };
    var grid = std.mem.zeroes([y_floor][1 + x_bound[1] - x_bound[0]]bool);
    line_it = std.mem.tokenize(u8, input, std.cstr.line_sep);
    while (line_it.next()) |line| {
        var pos_it: ?std.mem.SplitIterator(u8) = null;
        var pos = pathParseNext(line, &pos_it).?;
        while (pathParseNext(line, &pos_it)) |next_pos| {
            while (pos.x != next_pos.x or pos.y != next_pos.y) : ({
                pos.x += std.math.clamp(@as(i32, next_pos.x) - pos.x, -1, 1);
                pos.y += std.math.clamp(@as(i32, next_pos.y) - pos.y, -1, 1);
            }) grid[pos.y][pos.x - x_bound[0]] = true;
        }
        grid[pos.y][pos.x - x_bound[0]] = true;
    }
    break :blk .{ .grid = grid, .x_bound = x_bound, .x_p1bound = x_p1bound, .y_floor = y_floor };
};

pub fn main() !void {
    var p1: u32 = 0;
    var p2: u32 = 0;
    var grid = parsed.grid;
    while (!grid[0][500 - parsed.x_bound[0]]) : (p2 += 1) {
        var pos = Position{ .x = 500, .y = 0 };
        while (pos.y < parsed.y_floor - 1) {
            if (!grid[pos.y + 1][pos.x - parsed.x_bound[0]]) {
                pos.y += 1;
            } else if (pos.x - 1 < parsed.x_bound[0] or !grid[pos.y + 1][pos.x - 1 - parsed.x_bound[0]]) {
                pos.y += 1;
                pos.x -= 1;
            } else if (pos.x + 1 > parsed.x_bound[1] or !grid[pos.y + 1][pos.x + 1 - parsed.x_bound[0]]) {
                pos.y += 1;
                pos.x += 1;
            } else break;
            if (p1 == 0 and (pos.x < parsed.x_p1bound[0] or pos.x > parsed.x_p1bound[1] or pos.y >= parsed.y_floor - 2)) p1 = p2;
        }
        grid[pos.y][pos.x - parsed.x_bound[0]] = true;
    }
    const stdout = std.io.getStdOut().writer();
    try stdout.print("Day14 (comptime parsing): P1: {}, P2: {}\n", .{ p1, p2 });
}
