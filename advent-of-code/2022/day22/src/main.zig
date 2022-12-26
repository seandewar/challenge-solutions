///! This solution should work for any cube net; if it doesn't, I'll cry.
const std = @import("std");
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
    var regions = std.BoundedArray(@Vector(2, i32), 6).init(0) catch unreachable;
    var pos = @Vector(2, i32){ undefined, 0 };
    while (pos[1] < height) : (pos[1] += region_size) {
        pos[0] = 0;
        while (pos[0] < map[pos[1]].len) : (pos[0] += region_size) {
            if (map[pos[1]][pos[0]] != ' ') regions.append(@divExact(pos, @splat(2, @as(i32, region_size)))) catch unreachable;
        }
    }
    var faces = std.BoundedArray(Face, 6).init(0) catch unreachable;
    faces.append(.{ .region_i = 0, .normal = .{ 0, 0, -1 }, .up_dir = .{ 0, 1, 0 } }) catch unreachable; // Start from the back face.
    foldCube(&faces, regions.buffer); // Assume we're behind the cube facing towards the back: +x = to right, +y = to top, +z = to front.
    for (faces.slice()) |*face| { // Find adjacent faces and our new orientation after crossing edges.
        var side_dir = face.up_dir;
        for (dirs_2d) |_, dir_i| {
            if (dir_i > 0) side_dir = rotate90(side_dir, face.normal);
            const next_face_normal = rotate90(face.normal, cross(face.normal, side_dir));
            var next_face_i: usize = undefined;
            for (faces.slice()) |face_, face_i_| {
                if (@reduce(.Or, face_.normal != next_face_normal)) continue;
                next_face_i = face_i_;
                break;
            }
            var new_dir = rotate90(side_dir, cross(face.normal, side_dir));
            var new_dir_i = 0;
            while (@reduce(.Or, new_dir != faces.get(next_face_i).up_dir)) : (new_dir_i += 1)
                new_dir = rotate90(new_dir, -next_face_normal);
            face.dir_adj_faces[dir_i] = .{ .face_i = next_face_i, .new_dir_i = new_dir_i };
        }
    }
    break :blk .{
        .map = map,
        .cmds = cmds.slice(),
        .start_pos = start_pos,
        .region_size = region_size,
        .regions = regions.buffer,
        .p2_faces = faces.buffer,
    };
};

fn cross(a: @Vector(3, i32), b: @Vector(3, i32)) @Vector(3, i32) {
    return .{ a[1] * b[2] - b[1] * a[2], b[0] * a[2] - a[0] * b[2], a[0] * b[1] - b[0] * a[1] };
}
fn rotate90(v: @Vector(3, i32), k: @Vector(3, i32)) @Vector(3, i32) {
    return cross(v, k) * @splat(3, @as(i32, -1)) + k * (k * v);
}
const dirs_2d = [_]@Vector(2, i32){ .{ 0, -1 }, .{ 1, 0 }, .{ 0, 1 }, .{ -1, 0 } }; // Starts pointing up (-y); clockwise.
const Face = struct {
    region_i: usize,
    normal: @Vector(3, i32),
    up_dir: @Vector(3, i32),
    dir_adj_faces: [4]struct { face_i: usize, new_dir_i: usize } = undefined,
};
fn foldCube(faces: *std.BoundedArray(Face, 6), regions: [6]@Vector(2, i32)) void {
    const face = faces.get(faces.len - 1);
    dir_loop: for (dirs_2d) |dir| {
        const next_region_pos = regions[face.region_i] + dir;
        var next_region_i_opt: ?usize = null;
        for (regions) |pos, region_i| {
            if (@reduce(.Or, pos != next_region_pos)) continue;
            next_region_i_opt = region_i;
            break;
        }
        const next_region_i = next_region_i_opt orelse continue;
        for (faces.slice()) |face_| if (face_.region_i == next_region_i) continue :dir_loop;
        var axis = if (dir[0] != 0) face.up_dir else cross(face.up_dir, face.normal);
        if (dir[0] > 0 or dir[1] < 0) axis = -axis;
        faces.append(.{
            .region_i = next_region_i,
            .normal = rotate90(face.normal, axis),
            .up_dir = rotate90(face.up_dir, axis),
        }) catch unreachable;
        foldCube(faces, regions);
    }
}

fn mapInBounds(pos: @Vector(2, i32)) bool {
    return pos[0] >= 0 and pos[1] >= 0 and pos[1] < parsed_input.map.len and pos[0] < parsed_input.map[@intCast(usize, pos[1])].len and
        parsed_input.map[@intCast(usize, pos[1])][@intCast(usize, pos[0])] != ' ';
}
fn computeAnswer(p2_faces: ?[6]Face) i32 {
    var dir_i: usize = 1;
    var pos = parsed_input.start_pos;
    for (parsed_input.cmds) |cmd| {
        switch (cmd) {
            .steps => |steps| {
                var rem_steps = steps;
                while (rem_steps > 0) : (rem_steps -= 1) {
                    var next_pos = pos + dirs_2d[dir_i];
                    var next_dir_i = dir_i;
                    if (!mapInBounds(next_pos)) {
                        if (p2_faces) |faces| {
                            const region_size = @splat(2, @as(i32, parsed_input.region_size));
                            var region_i: usize = undefined;
                            for (parsed_input.regions) |region_pos, i| {
                                if (@reduce(.Or, @divTrunc(pos, region_size) != region_pos)) continue;
                                region_i = i;
                                break;
                            }
                            var face: Face = undefined;
                            for (faces) |face_| {
                                if (face_.region_i != region_i) continue;
                                face = face_;
                                break;
                            }
                            const adj_info = face.dir_adj_faces[dir_i];
                            const region_offset = @mod(next_pos, region_size);
                            const next_region_pos = parsed_input.regions[faces[adj_info.face_i].region_i];
                            var rot = @Vector(3, i32){ region_offset[0] + 1, region_offset[1] + 1, 0 };
                            while (next_dir_i != adj_info.new_dir_i) : (next_dir_i = (next_dir_i + 1) % dirs_2d.len)
                                rot = rotate90(rot, .{ 0, 0, 1 });
                            const next_region_offset = @mod(
                                @Vector(2, i32){ rot[0], rot[1] },
                                region_size + @splat(2, @as(i32, 1)),
                            ) - @splat(2, @as(i32, 1));
                            next_pos = next_region_pos * region_size + next_region_offset;
                        } else {
                            next_pos = pos;
                            while (mapInBounds(next_pos - dirs_2d[dir_i])) next_pos -= dirs_2d[dir_i];
                        }
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

const p1 = blk: {
    @setEvalBranchQuota(100_000);
    break :blk computeAnswer(null);
};
pub fn main() !void {
    const stdout = std.io.getStdOut().writer();
    try stdout.print("Day22 (P1 and P2 cube folding at comptime): P1: {}, P2: {}\n", .{ p1, computeAnswer(parsed_input.p2_faces) });
}
