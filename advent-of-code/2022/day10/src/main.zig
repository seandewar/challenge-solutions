const std = @import("std");

const answers = blk: {
    @setEvalBranchQuota(10_000);
    const input = @embedFile("input");
    var x = 1;
    var cycle = 0; // 0-based to simplify part 2 pixel putting.
    var pending_instr: ?struct { x_add: i8 = 0, cycles: u8 } = null;
    var p1 = 0;
    var p2: [6][40]u8 = undefined;
    p2[0][0] = '#'; // X starts at 1, so first pixel is always lit.
    var line_it = std.mem.tokenize(u8, input, "\n");
    while (pending_instr != null or line_it.peek() != null) {
        if (pending_instr == null) {
            const line = line_it.next().?;
            if (std.mem.startsWith(u8, line, "addx ")) {
                pending_instr = .{ .x_add = std.fmt.parseInt(i8, line[5..], 10) catch unreachable, .cycles = 2 };
            } else { // noop
                pending_instr = .{ .cycles = 1 };
            }
        }
        cycle += 1;
        pending_instr.?.cycles -= 1;
        if (pending_instr.?.cycles == 0) {
            x += pending_instr.?.x_add;
            pending_instr = null;
        }
        if ((cycle + 21) % 40 == 0) p1 += (cycle + 1) * x;
        if (cycle < 6 * 40) p2[cycle / 40][cycle % 40] = if (cycle % 40 >= x - 1 and cycle % 40 <= x + 1) '#' else ' ';
    }
    break :blk .{ p1, p2 };
};

pub fn main() !void {
    const stdout = std.io.getStdOut().writer();
    try stdout.print("Day10 (comptime only): P1: {}, P2:\n", .{answers[0]});
    for (answers[1]) |row| try stdout.print("{s}\n", .{row});
}
