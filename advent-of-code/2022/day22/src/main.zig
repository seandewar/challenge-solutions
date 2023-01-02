///! This solution should work for any cube net; if it doesn't, I'll cry.
const std = @import("std");
const Region = struct {
    pos: @Vector(2, i32),
    face_normal: @Vector(3, i32) = undefined,
    face_up_dir: @Vector(3, i32) = undefined,
    dir_adj_faces: [4]struct { region_i: usize, new_dir_i: usize } = undefined,
};
fn findRegionIndex(regions: [6]Region, value: union(enum) { pos: @Vector(2, i32), face_normal: @Vector(3, i32) }) ?usize {
    for (regions) |region, region_i| if (switch (value) {
        .pos => |pos| @reduce(.And, region.pos == pos),
        .face_normal => |normal| @reduce(.And, region.face_normal == normal),
    }) return region_i;
    return null;
}

fn cross(a: @Vector(3, i32), b: @Vector(3, i32)) @Vector(3, i32) {
    return .{ a[1] * b[2] - b[1] * a[2], b[0] * a[2] - a[0] * b[2], a[0] * b[1] - b[0] * a[1] };
}
fn rotate90(v: @Vector(3, i32), k: @Vector(3, i32)) @Vector(3, i32) {
    return cross(k, v) + k * (k * v);
}
const dirs_2d = [_]@Vector(2, i32){ .{ 0, -1 }, .{ 1, 0 }, .{ 0, 1 }, .{ -1, 0 } }; // Starts pointing up (-y); clockwise.
const parsed_input = blk: {
    @setEvalBranchQuota(1_000_000);
    const input = @embedFile("input");
    var height = 0;
    var line_it = std.mem.tokenize(u8, input, std.cstr.line_sep);
    while (line_it.next()) |_| height += 1;
    height -= 1; // Commands line doesn't count.
    var map: [height][]const u8 = undefined;
    var region_size = std.math.maxInt(usize);
    line_it.reset();
    for (map) |*row| {
        row.* = line_it.next().?;
        const first_i = std.mem.indexOfAny(u8, row.*, ".#") orelse row.len;
        region_size = @min(region_size, std.mem.indexOfScalarPos(u8, row.*, first_i, ' ') orelse row.len - first_i);
    }
    const cmds_line = line_it.next().?;
    var cmds = std.BoundedArray(union(enum) { steps: u32, dir: u8 }, cmds_line.len).init(0) catch unreachable;
    var i = 0;
    while (i < cmds_line.len) {
        if (std.ascii.isDigit(cmds_line[i])) {
            const end_i = std.mem.indexOfAnyPos(u8, cmds_line, i, "LR") orelse cmds_line.len;
            cmds.append(.{ .steps = std.fmt.parseInt(u32, cmds_line[i..end_i], 10) catch unreachable }) catch unreachable;
            i = end_i;
        } else {
            cmds.append(.{ .dir = cmds_line[i] }) catch unreachable;
            i += 1;
        }
    }
    const start_pos = @Vector(2, i32){ std.mem.indexOfScalar(u8, map[0], '.').?, 0 };
    var regions = std.BoundedArray(Region, 6).init(0) catch unreachable;
    var pos = @Vector(2, i32){ undefined, 0 };
    while (pos[1] < height) : (pos[1] += region_size) {
        pos[0] = 0;
        while (pos[0] < map[pos[1]].len) : (pos[0] += region_size) {
            if (map[pos[1]][pos[0]] != ' ')
                regions.append(.{ .pos = @divExact(pos, @splat(2, @as(i32, region_size))) }) catch unreachable;
        }
    }
    var visited_region_is = std.StaticBitSet(6).initEmpty();
    visited_region_is.set(0); // Start from the first region; consider it the back face.
    regions.buffer[0].face_normal = .{ 0, 0, -1 }; // Assume we're behind the cube facing forwards: +x = to right, +y = to top, +z = to front.
    regions.buffer[0].face_up_dir = .{ 0, 1, 0 };
    foldCube(&regions.buffer, 0, &visited_region_is);
    for (regions.buffer) |*region| { // Find adjacent faces and our new orientation after crossing edges.
        var side_dir = region.face_up_dir;
        for (dirs_2d) |_, dir_i| {
            if (dir_i > 0) side_dir = rotate90(side_dir, region.face_normal);
            const next_face_normal = rotate90(region.face_normal, cross(region.face_normal, side_dir));
            const next_region_i = findRegionIndex(regions.buffer, .{ .face_normal = next_face_normal }).?;
            var new_dir = rotate90(side_dir, cross(region.face_normal, side_dir));
            var new_dir_i = 0;
            while (@reduce(.Or, new_dir != regions.get(next_region_i).face_up_dir)) : (new_dir_i += 1)
                new_dir = rotate90(new_dir, -next_face_normal);
            region.dir_adj_faces[dir_i] = .{ .region_i = next_region_i, .new_dir_i = new_dir_i };
        }
    }
    break :blk .{ .map = map, .cmds = cmds.slice(), .start_pos = start_pos, .region_size = region_size, .regions = regions.buffer };
};
fn foldCube(regions: *[6]Region, region_i: usize, visited_region_is: *std.StaticBitSet(6)) void {
    const region = regions[region_i];
    for (dirs_2d) |dir| {
        const next_region_i = findRegionIndex(regions.*, .{ .pos = region.pos + dir }) orelse continue;
        if (visited_region_is.isSet(next_region_i)) continue;
        var axis = if (dir[0] != 0) region.face_up_dir else cross(region.face_up_dir, region.face_normal);
        if (dir[0] > 0 or dir[1] < 0) axis = -axis;
        regions[next_region_i].face_normal = rotate90(region.face_normal, axis);
        regions[next_region_i].face_up_dir = rotate90(region.face_up_dir, axis);
        visited_region_is.set(next_region_i);
        foldCube(regions, next_region_i, visited_region_is);
    }
}

fn mapInBounds(pos: @Vector(2, i32)) bool {
    return pos[0] >= 0 and pos[1] >= 0 and pos[1] < parsed_input.map.len and pos[0] < parsed_input.map[@intCast(usize, pos[1])].len and
        parsed_input.map[@intCast(usize, pos[1])][@intCast(usize, pos[0])] != ' ';
}
fn computeAnswer(comptime is_p1: bool) i32 {
    var dir_i: usize = 1;
    var pos = parsed_input.start_pos;
    for (parsed_input.cmds) |cmd| {
        switch (cmd) {
            .steps => |steps| {
                var rem_steps = steps;
                while (rem_steps > 0) : (rem_steps -= 1) {
                    var next_pos = pos + dirs_2d[dir_i];
                    var next_dir_i = dir_i;
                    if (!mapInBounds(next_pos)) step: {
                        if (is_p1) { // P1 is pretty simple; just wrap around to the opposite side.
                            next_pos = pos;
                            while (mapInBounds(next_pos - dirs_2d[dir_i])) next_pos -= dirs_2d[dir_i];
                            break :step;
                        }
                        const region_size = @splat(2, @as(i32, parsed_input.region_size));
                        const region_i = findRegionIndex(parsed_input.regions, .{ .pos = @divTrunc(pos, region_size) }).?;
                        const adj_info = parsed_input.regions[region_i].dir_adj_faces[dir_i];
                        const region_offset = @mod(next_pos, region_size);
                        var rot = @Vector(3, i32){ region_offset[0] + 1, region_offset[1] + 1, 0 };
                        while (next_dir_i != adj_info.new_dir_i) : (next_dir_i = (next_dir_i + 1) % dirs_2d.len)
                            rot = rotate90(rot, .{ 0, 0, 1 });
                        next_pos = parsed_input.regions[adj_info.region_i].pos * region_size + @mod(
                            @Vector(2, i32){ rot[0], rot[1] },
                            region_size + @splat(2, @as(i32, 1)),
                        ) - @splat(2, @as(i32, 1));
                    }
                    if (parsed_input.map[@intCast(usize, next_pos[1])][@intCast(usize, next_pos[0])] == '#') break;
                    pos = next_pos;
                    dir_i = next_dir_i;
                }
            },
            .dir => |c| dir_i = (if (c == 'L') dir_i + dirs_2d.len - 1 else dir_i + 1) % dirs_2d.len,
        }
    }
    return 1000 * (pos[1] + 1) + 4 * (pos[0] + 1) + @mod(@intCast(i32, dir_i) - 1, 4);
}
const answers = blk: {
    @setEvalBranchQuota(1_000_000);
    break :blk .{ computeAnswer(true), computeAnswer(false) };
};
pub fn main() !void {
    const stdout = std.io.getStdOut().writer();
    try stdout.print("Day22 (comptime only): P1: {}, P2: {}\n", answers);
}
