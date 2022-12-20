const std = @import("std");

const Vector = struct { x: i32 = 0, y: i32 = 0, z: i32 = 0 };
fn gridIndexInBounds(i: Vector, size: Vector) bool {
    return i.x >= 0 and i.y >= 0 and i.z >= 0 and i.x < size.x and i.y < size.y and i.z < size.z;
}
fn parseNextCube(line: []const u8) Vector {
    var it = std.mem.split(u8, line, ",");
    return .{
        .x = std.fmt.parseInt(i32, it.next().?, 10) catch unreachable,
        .y = std.fmt.parseInt(i32, it.next().?, 10) catch unreachable,
        .z = std.fmt.parseInt(i32, it.next().?, 10) catch unreachable,
    };
}
const parsed_input = blk: {
    @setEvalBranchQuota(1_000_000);
    const input = @embedFile("input");
    var bounds = [_]Vector{.{}} ** 2;
    var pos_count = 0;
    var line_it = std.mem.tokenize(u8, input, std.cstr.line_sep);
    while (line_it.next()) |line| {
        const pos = parseNextCube(line);
        for (@typeInfo(Vector).Struct.fields) |field| {
            @field(bounds[0], field.name) = @min(@field(bounds[0], field.name), @field(pos, field.name));
            @field(bounds[1], field.name) = @max(@field(bounds[1], field.name), @field(pos, field.name));
        }
        pos_count += 1;
    }
    const size = Vector{
        .x = bounds[1].x - bounds[0].x + 1,
        .y = bounds[1].y - bounds[0].y + 1,
        .z = bounds[1].z - bounds[0].z + 1,
    };
    var grid = std.mem.zeroes([size.z][size.y][size.x]usize);
    var positions: [pos_count]Vector = undefined;
    line_it.reset();
    for (positions) |*pos| {
        pos.* = parseNextCube(line_it.next().?);
        grid[pos.z - bounds[0].z][pos.y - bounds[0].y][pos.x - bounds[0].x] = 1;
    }
    var p1 = 6 * positions.len;
    for (positions) |pos| {
        for ([_]Vector{ .{ .x = -1 }, .{ .x = 1 }, .{ .y = -1 }, .{ .y = 1 }, .{ .z = -1 }, .{ .z = 1 } }) |dir| {
            const i = Vector{
                .x = pos.x + dir.x - bounds[0].x,
                .y = pos.y + dir.y - bounds[0].y,
                .z = pos.z + dir.z - bounds[0].z,
            };
            if (gridIndexInBounds(i, size) and grid[i.z][i.y][i.x] == 1) p1 -= 1;
        }
    }
    break :blk .{ .p1 = p1, .grid = grid, .size = size, .bounds = bounds, .positions = positions };
};

fn floodCavity(grid: *@TypeOf(parsed_input.grid), i: Vector, value: usize) ?u32 {
    grid[@intCast(usize, i.z)][@intCast(usize, i.y)][@intCast(usize, i.x)] = value;
    var surface_area: u32 = 0;
    for ([_]Vector{ .{ .x = -1 }, .{ .x = 1 }, .{ .y = -1 }, .{ .y = 1 }, .{ .z = -1 }, .{ .z = 1 } }) |dir| {
        const j = Vector{
            .x = i.x + dir.x,
            .y = i.y + dir.y,
            .z = i.z + dir.z,
        };
        if (!gridIndexInBounds(j, parsed_input.size)) return null;
        const j_value = grid[@intCast(usize, j.z)][@intCast(usize, j.y)][@intCast(usize, j.x)];
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
        for ([_]Vector{ .{ .x = -1 }, .{ .x = 1 }, .{ .y = -1 }, .{ .y = 1 }, .{ .z = -1 }, .{ .z = 1 } }) |dir, dir_i| {
            const i = Vector{
                .x = pos.x + dir.x - parsed_input.bounds[0].x,
                .y = pos.y + dir.y - parsed_input.bounds[0].y,
                .z = pos.z + dir.z - parsed_input.bounds[0].z,
            };
            if (!gridIndexInBounds(i, parsed_input.size)) continue;
            if (grid[@intCast(usize, i.z)][@intCast(usize, i.y)][@intCast(usize, i.x)] != 0) continue;
            if (floodCavity(&grid, i, 2 + 6 * pos_i + dir_i)) |surface_area| p2 -= surface_area;
        }
    }
    const stdout = std.io.getStdOut().writer();
    try stdout.print("Day18 (P1 at comptime): P1: {}, P2: {}\n", .{ parsed_input.p1, p2 });
}
