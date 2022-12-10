const std = @import("std");

const answers = blk: {
    @setEvalBranchQuota(10_000);
    const input = @embedFile("input");
    var x = 1;
    var cycle = 0; // 0-based to simplify part 2 pixel putting.
    var instr: struct { addx: i8 = 0, cycles: u8 = 0 } = .{};
    var p1 = 0;
    var p2 = [_][40]u8{.{'#'} ** 40} ** 6; // Assume lit (slightly simplifies P2).
    var line_it = std.mem.tokenize(u8, input, std.cstr.line_sep);
    while (instr.cycles != 0 or line_it.peek() != null) {
        if (instr.cycles == 0) {
            const line = line_it.next().?;
            if (std.mem.startsWith(u8, line, "addx ")) {
                instr = .{ .addx = std.fmt.parseInt(i8, line[5..], 10) catch unreachable, .cycles = 2 };
            } else { // noop
                instr = .{ .cycles = 1 };
            }
        }
        cycle += 1;
        instr.cycles -= 1;
        if (instr.cycles == 0) x += instr.addx;
        if ((cycle + 21) % 40 == 0) p1 += (cycle + 1) * x;
        if (cycle < 6 * 40 and (cycle % 40 < x - 1 or cycle % 40 > x + 1)) p2[cycle / 40][cycle % 40] = ' ';
    }
    break :blk .{ p1, p2 };
};

pub fn main() !void {
    const stdout = std.io.getStdOut().writer();
    try stdout.print("Day10 (comptime only): P1: {}, P2:\n", .{answers[0]});
    for (answers[1]) |row| try stdout.print("{s}\n", .{row});
}
