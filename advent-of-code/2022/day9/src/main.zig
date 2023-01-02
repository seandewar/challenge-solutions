const std = @import("std");
fn parseCommand(cmd: []const u8) struct { dir: @Vector(2, i32), steps: u8 } {
    const dir: @Vector(2, i32) = switch (cmd[0]) {
        'R' => .{ 1, 0 },
        'L' => .{ -1, 0 },
        'U' => .{ 0, 1 },
        'D' => .{ 0, -1 },
        else => unreachable,
    };
    const steps = std.fmt.parseInt(u8, cmd[2..], 10) catch unreachable;
    return .{ .dir = dir, .steps = steps };
}
const answers = blk: {
    @setEvalBranchQuota(1_000_000);
    const input = @embedFile("input");
    var rope_pos = [_]@Vector(2, i32){.{ 0, 0 }} ** 10;
    var min_pos = @Vector(2, i32){ 0, 0 };
    var max_pos = @Vector(2, i32){ 0, 0 };
    var line_it = std.mem.tokenize(u8, input, std.cstr.line_sep);
    while (line_it.next()) |line| { // Compute grid bounds for marking visited tail positions.
        const cmd = parseCommand(line);
        rope_pos[0] += cmd.dir * @splat(2, @as(i32, cmd.steps));
        min_pos = @min(min_pos, rope_pos[0]);
        max_pos = @max(max_pos, rope_pos[0]);
    }
    rope_pos[0] = .{ 0, 0 };
    var tail_visit_grid = std.mem.zeroes([1 + max_pos[1] - min_pos[1]][1 + max_pos[0] - min_pos[0]]u2);
    tail_visit_grid[-min_pos[1]][-min_pos[0]] = 0b11;
    line_it.reset();
    while (line_it.next()) |line| {
        var cmd = parseCommand(line);
        while (cmd.steps != 0) : (cmd.steps -= 1) {
            rope_pos[0] += cmd.dir; // Move head part (H).
            for (rope_pos[1..]) |*pos, tail_i| { // Move tails parts (1-9).
                const delta = rope_pos[tail_i] - pos.*;
                if (std.math.absCast(delta[0]) < 2 and std.math.absCast(delta[1]) < 2) break; // Adjacent to previous part - no movement.
                pos.* += @Vector(2, i32){ std.math.clamp(delta[0], -1, 1), std.math.clamp(delta[1], -1, 1) };
                if (tail_i == 0) tail_visit_grid[pos.*[1] - min_pos[1]][pos.*[0] - min_pos[0]] |= 0b01; // First part of tail (1) visited.
                if (tail_i == 8) tail_visit_grid[pos.*[1] - min_pos[1]][pos.*[0] - min_pos[0]] |= 0b10; // Last part of tail (9) visited.
            }
        }
    }
    var p1: u32 = 0;
    var p2: u32 = 0;
    for (tail_visit_grid) |row| {
        for (row) |visited| {
            p1 += visited & 1;
            p2 += (visited >> 1) & 1;
        }
    }
    break :blk .{ p1, p2 };
};

pub fn main() !void {
    const stdout = std.io.getStdOut().writer();
    try stdout.print("Day9 (comptime only): P1: {}, P2: {}\n", answers);
}
