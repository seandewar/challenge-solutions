const std = @import("std");
fn gridIndexInBounds(i: @Vector(3, i32), size: @Vector(3, i32)) bool {
    return i[0] >= 0 and i[1] >= 0 and i[2] >= 0 and i[0] < size[0] and i[1] < size[1] and i[2] < size[2];
}
fn parseNextCube(line: []const u8) @Vector(3, i32) {
    var it = std.mem.split(u8, line, ",");
    return .{
        std.fmt.parseInt(i32, it.next().?, 10) catch unreachable,
        std.fmt.parseInt(i32, it.next().?, 10) catch unreachable,
        std.fmt.parseInt(i32, it.next().?, 10) catch unreachable,
    };
}
const dirs = [_]@Vector(3, i32){ .{ -1, 0, 0 }, .{ 1, 0, 0 }, .{ 0, -1, 0 }, .{ 0, 1, 0 }, .{ 0, 0, -1 }, .{ 0, 0, 1 } };
const parsed_input = blk: {
    @setEvalBranchQuota(1_000_000);
    const input = @embedFile("input");
    var bounds = [_]@Vector(3, i32){@splat(3, @as(i32, 0))} ** 2;
    var pos_count = 0;
    var line_it = std.mem.tokenize(u8, input, std.cstr.line_sep);
    while (line_it.next()) |line| : (pos_count += 1) {
        const pos = parseNextCube(line);
        bounds[0] = @min(bounds[0], pos);
        bounds[1] = @max(bounds[1], pos);
    }
    const size = bounds[1] - bounds[0] + @splat(3, @as(i32, 1));
    var grid = std.mem.zeroes([size[2]][size[1]][size[0]]usize);
    var positions: [pos_count]@Vector(3, i32) = undefined;
    line_it.reset();
    for (positions) |*pos| {
        pos.* = parseNextCube(line_it.next().?);
        const i = pos.* - bounds[0];
        grid[i[2]][i[1]][i[0]] = 1;
    }
    var p1 = 6 * positions.len;
    for (positions) |pos| {
        for (dirs) |dir| {
            const i = pos + dir - bounds[0];
            if (gridIndexInBounds(i, size) and grid[i[2]][i[1]][i[0]] == 1) p1 -= 1;
        }
    }
    break :blk .{ .p1 = p1, .grid = grid, .size = size, .bounds = bounds, .positions = positions };
};

fn floodCavity(grid: *@TypeOf(parsed_input.grid), i: @Vector(3, i32), value: usize) ?u32 {
    grid[@intCast(usize, i[2])][@intCast(usize, i[1])][@intCast(usize, i[0])] = value;
    var surface_area: u32 = 0;
    for (dirs) |dir| {
        const j = i + dir;
        if (!gridIndexInBounds(j, parsed_input.size)) return null;
        const j_value = grid[@intCast(usize, j[2])][@intCast(usize, j[1])][@intCast(usize, j[0])];
        if (j_value == 1) surface_area += 1;
        if (j_value == value or j_value == 1) continue;
        if (j_value != 0) return null;
        surface_area += floodCavity(grid, j, value) orelse return null;
    }
    return surface_area;
}
pub fn main() !void {
    var p2: u32 = parsed_input.p1;
    var grid = parsed_input.grid;
    for (parsed_input.positions) |pos, pos_i| {
        for (dirs) |dir, dir_i| {
            const i = pos + dir - parsed_input.bounds[0];
            if (!gridIndexInBounds(i, parsed_input.size)) continue;
            if (grid[@intCast(usize, i[2])][@intCast(usize, i[1])][@intCast(usize, i[0])] != 0) continue;
            if (floodCavity(&grid, i, 2 + 6 * pos_i + dir_i)) |surface_area| p2 -= surface_area;
        }
    }
    const stdout = std.io.getStdOut().writer();
    try stdout.print("Day18 (P1 at comptime): P1: {}, P2: {}\n", .{ parsed_input.p1, p2 });
}
