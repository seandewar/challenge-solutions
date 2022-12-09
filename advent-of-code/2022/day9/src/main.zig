const std = @import("std");

const Position = struct {
    x: i32 = 0,
    y: i32 = 0,
};
fn parseCommand(cmd: []const u8) struct { dir: Position, steps: u8 } {
    const dir: Position = switch (cmd[0]) {
        'R' => .{ .x = 1 },
        'L' => .{ .x = -1 },
        'U' => .{ .y = 1 },
        'D' => .{ .y = -1 },
        else => unreachable,
    };
    const steps = std.fmt.parseInt(u8, cmd[2..], 10) catch unreachable;
    return .{ .dir = dir, .steps = steps };
}

const answers = blk: {
    @setEvalBranchQuota(1_000_000);
    const input = @embedFile("input");
    var rope_pos = [_]Position{.{}} ** 10;
    var min_pos = Position{};
    var max_pos = Position{};
    var line_it = std.mem.tokenize(u8, input, "\n");
    while (line_it.next()) |line| {
        const cmd = parseCommand(line);
        rope_pos[0].x += cmd.dir.x * cmd.steps;
        rope_pos[0].y += cmd.dir.y * cmd.steps;
        min_pos.x = @min(min_pos.x, rope_pos[0].x);
        min_pos.y = @min(min_pos.y, rope_pos[0].y);
        max_pos.x = @max(max_pos.x, rope_pos[0].x);
        max_pos.y = @max(max_pos.y, rope_pos[0].y);
    }
    rope_pos[0] = .{};
    var tail_visit_grid = std.mem.zeroes([1 + max_pos.y - min_pos.y][1 + max_pos.x - min_pos.x]u2);
    tail_visit_grid[0][0] = 0b11;
    line_it = std.mem.tokenize(u8, input, "\n");
    while (line_it.next()) |line| {
        var cmd = parseCommand(line);
        while (cmd.steps != 0) : (cmd.steps -= 1) {
            rope_pos[0].x += cmd.dir.x;
            rope_pos[0].y += cmd.dir.y;
            for (rope_pos[1..]) |*pos, i| {
                const delta = Position{ .x = rope_pos[i].x - pos.x, .y = rope_pos[i].y - pos.y };
                if (std.math.absCast(delta.x) < 2 and std.math.absCast(delta.y) < 2) break;
                pos.x += std.math.clamp(delta.x, -1, 1);
                pos.y += std.math.clamp(delta.y, -1, 1);
                if (i == 0) tail_visit_grid[pos.y - min_pos.y][pos.x - min_pos.x] |= 0b01;
                if (i == 8) tail_visit_grid[pos.y - min_pos.y][pos.x - min_pos.x] |= 0b10;
            }
        }
    }
    var p1: u32 = 0;
    var p2: u32 = 0;
    for (tail_visit_grid) |row| {
        for (row) |visited| {
            p1 += visited & 1;
            if (visited & 0b10 != 0) p2 += 1;
        }
    }
    break :blk .{ p1, p2 };
};

pub fn main() !void {
    const stdout = std.io.getStdOut().writer();
    try stdout.print("Day9 (comptime only): P1: {}, P2: {}\n", answers);
}
