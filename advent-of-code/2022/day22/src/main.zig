const std = @import("std");
const Position = struct { x: i32 = 0, y: i32 = 0 };
const parsed_input = blk: {
    @setEvalBranchQuota(1_000_000);
    const input = @embedFile("input");
    var height = 0;
    var line_it = std.mem.tokenize(u8, input, std.cstr.line_sep);
    while (line_it.next()) |_| height += 1;
    height -= 1; // Commands line doesn't count.
    var map: [height][]const u8 = undefined;
    line_it.reset();
    for (map) |*row| row.* = line_it.next().?;
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
    const start_pos = Position{ .x = std.mem.indexOfScalar(u8, map[0], '.').? };
    break :blk .{ .map = map, .cmds = cmds.slice(), .start_pos = start_pos };
};

fn mapInBounds(pos: Position) bool {
    return pos.x >= 0 and pos.y >= 0 and
        pos.y < parsed_input.map.len and pos.x < parsed_input.map[@intCast(usize, pos.y)].len and
        parsed_input.map[@intCast(usize, pos.y)][@intCast(usize, pos.x)] != ' ';
}
const p1 = blk: {
    @setEvalBranchQuota(100_000);
    const dirs = [_]Position{ .{ .x = 1 }, .{ .y = 1 }, .{ .x = -1 }, .{ .y = -1 } };
    var dir_i = 0;
    var pos = parsed_input.start_pos;
    for (parsed_input.cmds) |cmd| {
        switch (cmd) {
            .steps => |steps| {
                const dir = dirs[dir_i];
                var rem_steps = steps;
                while (rem_steps > 0) : (rem_steps -= 1) {
                    var next_pos = Position{ .x = pos.x + dir.x, .y = pos.y + dir.y };
                    if (!mapInBounds(next_pos)) {
                        next_pos = pos;
                        while (mapInBounds(.{ .x = next_pos.x - dir.x, .y = next_pos.y - dir.y })) {
                            next_pos.x -= dir.x;
                            next_pos.y -= dir.y;
                        }
                    }
                    if (parsed_input.map[next_pos.y][next_pos.x] == '#') break;
                    pos = next_pos;
                }
            },
            .dir => |c| dir_i = (if (c == 'L') dir_i + dirs.len - 1 else dir_i + 1) % dirs.len,
        }
    }
    break :blk 1000 * (pos.y + 1) + 4 * (pos.x + 1) + dir_i;
};
pub fn main() !void {
    const stdout = std.io.getStdOut().writer();
    try stdout.print("Day22 (comptime only): P1: {}\n", .{p1});
}
