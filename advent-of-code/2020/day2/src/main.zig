///! Computed fully at compile time!
const std = @import("std");

pub fn main() !void {
    const results = comptime blk: {
        @setEvalBranchQuota(1_000_000);
        const input = @embedFile("input.txt");

        var day1_result = 0;
        var day2_result = 0;
        var iter = std.mem.tokenize(u8, input, "\n");
        while (iter.next()) |line| {
            const b_i = std.mem.indexOfScalar(u8, line, '-').? + 1;
            const letter_i = std.mem.indexOfScalarPos(u8, line, b_i + 1, ':').? - 1;
            const password_i = letter_i + 3;

            const a = std.fmt.parseUnsigned(u8, line[0 .. b_i - 1], 10) catch unreachable;
            const b = std.fmt.parseUnsigned(u8, line[b_i .. letter_i - 1], 10) catch unreachable;
            const letter = line[letter_i];
            const password = line[password_i..];

            const letter_count = std.mem.count(u8, password, &.{letter});
            if (letter_count >= a and letter_count <= b) {
                day1_result += 1;
            }

            const a_has = a <= password.len and password[a - 1] == letter;
            const b_has = b <= password.len and password[b - 1] == letter;
            if (a_has != b_has) {
                day2_result += 1;
            }
        }

        break :blk .{ day1_result, day2_result };
    };

    const stdout = std.io.getStdOut().writer();
    try stdout.print("Day 2:\nP1: {d}\nP2: {d}\n", results);
}
