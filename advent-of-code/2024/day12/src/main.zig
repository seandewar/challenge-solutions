// Code is a bit ugly as I forgot to do this day's problem and now I have to rush. >:(
const std = @import("std");

pub fn main() !void {
    var arena_instance: std.heap.ArenaAllocator = .init(std.heap.page_allocator);
    defer arena_instance.deinit();
    const arena = arena_instance.allocator();

    const grid: Grid = grid: {
        const plants = try std.fs.cwd().readFileAlloc(arena, "src/input", std.math.maxInt(usize));
        const width = if (std.mem.indexOfScalar(u8, plants, '\n')) |i| i + 1 else plants.len;
        break :grid .{
            .plants = plants,
            .width = width,
            .height = std.math.divCeil(usize, @max(1, plants.len), width) catch unreachable,
        };
    };

    var p1: usize = 0;
    var p2: usize = 0;
    const filled_regions = try arena.alloc(usize, grid.plants.len);
    @memset(filled_regions, 0);
    var next_region_num: usize = 1;
    const p2_visited = try arena.alloc(bool, (sides_res_mul * grid.width + 2) * (sides_res_mul * grid.height + 2));
    for (0..grid.height) |y_usize| {
        for (0..grid.width) |x_usize| {
            const x: isize = @intCast(x_usize);
            const y: isize = @intCast(y_usize);
            const i = grid.index(x, y) orelse break;
            if (!std.ascii.isAlphabetic(grid.plants[i]) or filled_regions[i] != 0) continue;

            const result = fillRegion(grid, x, y, grid.plants[i], filled_regions, next_region_num);
            p1 += result.area * result.perimeter;
            @memset(p2_visited, false);
            const sides = countSides(grid, sides_res_mul * x, sides_res_mul * y, filled_regions, next_region_num, p2_visited);
            p2 += result.area * sides;
            next_region_num += 1;
        }
    }

    try std.io.getStdOut().writer().print("Day12: P1: {}, P2: {}\n", .{ p1, p2 });
}

const Grid = struct {
    plants: []const u8,
    width: usize,
    height: usize,

    fn index(self: Grid, x: isize, y: isize) ?usize {
        if (x < 0 or y < 0 or x >= self.width or y >= self.height) return null;
        const i = @as(usize, @intCast(y)) * self.width + @as(usize, @intCast(x));
        return if (i < self.plants.len) i else null;
    }
};

const dirs_4: [4][2]i8 = .{ .{ 1, 0 }, .{ 0, 1 }, .{ -1, 0 }, .{ 0, -1 } };

const FillRegionResult = struct {
    area: usize,
    perimeter: usize,
};

fn fillRegion(
    grid: Grid,
    x: isize,
    y: isize,
    plant: u8,
    filled_regions: []usize,
    region_num: usize,
) FillRegionResult {
    var ret: FillRegionResult = .{ .area = 0, .perimeter = 0 };
    const i = grid.index(x, y) orelse return ret;
    if (grid.plants[i] != plant or filled_regions[i] != 0) return ret;
    filled_regions[i] = region_num;
    ret.area += 1;

    inline for (dirs_4) |dir| {
        const result = fillRegion(grid, x + dir[0], y + dir[1], plant, filled_regions, region_num);
        ret.area += result.area;
        ret.perimeter += result.perimeter;
    }
    inline for (dirs_4) |dir| {
        const j = grid.index(x + dir[0], y + dir[1]);
        if (j == null or filled_regions[j.?] != region_num)
            ret.perimeter += 1;
    }
    return ret;
}

const sides_res_mul = 3;

fn countSides(
    grid: Grid,
    x: isize,
    y: isize,
    filled_regions: []const usize,
    region_num: usize,
    visited: []bool,
) usize {
    if (x < -1 or y < -1 or x > sides_res_mul * grid.width or y > sides_res_mul * grid.height) return 0;
    const visited_i = @as(usize, @intCast(y + 1)) * (sides_res_mul * grid.width + 2) + @as(usize, @intCast(x + 1));
    if (visited[visited_i]) return 0;

    const grid_i = grid.index(@divFloor(x, sides_res_mul), @divFloor(y, sides_res_mul));
    if (grid_i == null or filled_regions[grid_i.?] != region_num) // are we now outside the region?
        return fillOutsideEdges(grid, x, y, filled_regions, region_num, visited, null);

    visited[visited_i] = true;
    var ret: usize = 0;
    inline for (dirs_4) |dir|
        ret += countSides(grid, x + dir[0], y + dir[1], filled_regions, region_num, visited);
    return ret;
}

const FillDir = enum { horizontal, vertical };

fn fillOutsideEdges(
    grid: Grid,
    x: isize,
    y: isize,
    filled_regions: []const usize,
    region_num: usize,
    visited: []bool,
    fill_dir: ?struct { prev: FillDir, curr: FillDir },
) usize {
    if (x < -1 or y < -1 or x > sides_res_mul * grid.width or y > sides_res_mul * grid.height) return 0;
    const visited_i = @as(usize, @intCast(y + 1)) * (sides_res_mul * grid.width + 2) + @as(usize, @intCast(x + 1));
    if (visited[visited_i]) return 0;

    // ensure we're on the outer edge of our region
    if (grid.index(@divFloor(x, sides_res_mul), @divFloor(y, sides_res_mul))) |grid_i|
        if (filled_regions[grid_i] == region_num) return 0;
    inline for (dirs_4 ++ .{ .{ 1, 1 }, .{ -1, 1 }, .{ 1, -1 }, .{ -1, -1 } }) |dir| {
        const grid_i = grid.index(@divFloor(x + dir[0], sides_res_mul), @divFloor(y + dir[1], sides_res_mul)) orelse comptime continue;
        if (filled_regions[grid_i] == region_num) break;
    } else return 0;

    if (fill_dir == null) {
        // ensure we start at a corner, or we'll count the sides incorrectly
        var edge_dir_i: ?usize = null;
        inline for (dirs_4, 0..) |dir, dir_i| {
            const grid_i = grid.index(@divFloor(x + dir[0], sides_res_mul), @divFloor(y + dir[1], sides_res_mul)) orelse comptime continue;
            if (filled_regions[grid_i] == region_num) {
                if (edge_dir_i != null) {
                    edge_dir_i = null;
                    break;
                }
                edge_dir_i = dir_i;
            }
        }
        if (edge_dir_i) |dir_i| {
            const dir = dirs_4[dir_i];
            return fillOutsideEdges(grid, x + dir[1], y + dir[0], filled_regions, region_num, visited, null);
        }
    }

    visited[visited_i] = true;
    var ret: usize = if (fill_dir) |fdir| @intFromBool(fdir.prev != fdir.curr) else 1;
    inline for (dirs_4) |dir| {
        const next_fill_dir: FillDir = if (dir[0] != 0) .horizontal else .vertical;
        const prev_fill_dir: FillDir = if (fill_dir) |fdir| fdir.curr else next_fill_dir;
        ret += fillOutsideEdges(grid, x + dir[0], y + dir[1], filled_regions, region_num, visited, .{ .prev = prev_fill_dir, .curr = next_fill_dir });
    }
    return ret;
}
