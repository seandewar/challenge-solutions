const std = @import("std");

const Vector = struct { x: i32 = 0, y: i32 = 0, z: i32 = 0 };
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
    var grid = std.mem.zeroes([size.z][size.y][size.x]bool);
    var positions: [pos_count]Vector = undefined;
    line_it.reset();
    for (positions) |*pos| {
        pos.* = parseNextCube(line_it.next().?);
        grid[pos.z - bounds[0].z][pos.y - bounds[0].y][pos.x - bounds[0].x] = true;
    }
    break :blk .{ .grid = grid, .size = size, .bounds = bounds, .positions = positions };
};

const p1 = blk: {
    @setEvalBranchQuota(100_000);
    var result = 6 * parsed_input.positions.len;
    for (parsed_input.positions) |pos| {
        for ([_]Vector{ .{ .x = -1 }, .{ .x = 1 }, .{ .y = -1 }, .{ .y = 1 }, .{ .z = -1 }, .{ .z = 1 } }) |dir| {
            const i = Vector{
                .x = pos.x + dir.x - parsed_input.bounds[0].x,
                .y = pos.y + dir.y - parsed_input.bounds[0].y,
                .z = pos.z + dir.z - parsed_input.bounds[0].z,
            };
            if (i.x < 0 or i.y < 0 or i.z < 0) continue;
            if (i.x >= parsed_input.size.x or i.y >= parsed_input.size.y or i.z >= parsed_input.size.z) continue;
            if (parsed_input.grid[i.z][i.y][i.x]) result -= 1;
        }
    }
    break :blk result;
};
pub fn main() !void {
    const stdout = std.io.getStdOut().writer();
    try stdout.print("Day18 (comptime only): P1: {}\n", .{p1});
}
