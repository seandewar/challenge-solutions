const std = @import("std");
fn pathParseNext(line: []const u8, it: *?std.mem.SplitIterator(u8)) ?@Vector(2, u16) {
    if (it.* == null) it.* = std.mem.split(u8, line, " -> ");
    const pos = it.*.?.next() orelse return null;
    const comma_i = std.mem.indexOfScalar(u8, pos, ',').?;
    return .{
        std.fmt.parseInt(u16, pos[0..comma_i], 10) catch unreachable,
        std.fmt.parseInt(u16, pos[comma_i + 1 ..], 10) catch unreachable,
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
            x_p1bound = .{ @min(x_p1bound[0], pos[0]), @max(x_p1bound[1], pos[0]) };
            y_floor = @max(y_floor, pos[1] + 2);
        }
    }
    const x_bound = [_]u16{ 501 - y_floor, 499 + y_floor };
    var grid = std.mem.zeroes([y_floor][1 + x_bound[1] - x_bound[0]]bool);
    line_it.reset();
    while (line_it.next()) |line| {
        var pos_it: ?std.mem.SplitIterator(u8) = null;
        var pos = pathParseNext(line, &pos_it).?;
        while (pathParseNext(line, &pos_it)) |next_pos| {
            while (@reduce(.Or, pos != next_pos)) : ({
                pos[0] += std.math.clamp(@as(i32, next_pos[0]) - pos[0], -1, 1);
                pos[1] += std.math.clamp(@as(i32, next_pos[1]) - pos[1], -1, 1);
            }) grid[pos[1]][pos[0] - x_bound[0]] = true;
        }
        grid[pos[1]][pos[0] - x_bound[0]] = true;
    }
    break :blk .{ .grid = grid, .x_bound = x_bound, .x_p1bound = x_p1bound, .y_floor = y_floor };
};

pub fn main() !void {
    var p1: u32 = 0;
    var p2: u32 = 0;
    var grid = parsed.grid;
    while (!grid[0][500 - parsed.x_bound[0]]) : (p2 += 1) {
        var pos = @Vector(2, u16){ 500, 0 };
        while (pos[1] < parsed.y_floor - 1) {
            if (!grid[pos[1] + 1][pos[0] - parsed.x_bound[0]]) {
                pos[1] += 1;
            } else if (pos[0] - 1 < parsed.x_bound[0] or !grid[pos[1] + 1][pos[0] - 1 - parsed.x_bound[0]]) {
                pos[0] -= 1;
                pos[1] += 1;
            } else if (pos[0] + 1 > parsed.x_bound[1] or !grid[pos[1] + 1][pos[0] + 1 - parsed.x_bound[0]]) {
                pos += @Vector(2, u16){ 1, 1 };
            } else break;
            if (p1 == 0 and (pos[0] < parsed.x_p1bound[0] or pos[0] > parsed.x_p1bound[1] or pos[1] >= parsed.y_floor - 2)) p1 = p2;
        }
        grid[pos[1]][pos[0] - parsed.x_bound[0]] = true;
    }
    const stdout = std.io.getStdOut().writer();
    try stdout.print("Day14 (comptime parsing): P1: {}, P2: {}\n", .{ p1, p2 });
}
