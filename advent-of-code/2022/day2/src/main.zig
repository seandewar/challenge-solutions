const std = @import("std");

const answers = blk: {
    @setEvalBranchQuota(100_000);
    const input = @embedFile("input");
    var line_it = std.mem.tokenize(u8, input, std.cstr.line_sep);
    var p1: u32 = 0;
    var p2: u32 = 0;
    while (line_it.next()) |line| {
        const p1_choice_score = 1 + line[2] - 'X';
        const p1_outcome_score = 3 * ((4 + line[2] - 'X' - (line[0] - 'A')) % 3);
        p1 += p1_choice_score + p1_outcome_score;
        const p2_choice_score = 1 + ((2 + line[0] - 'A' + line[2] - 'X') % 3);
        const p2_outcome_score = 3 * (line[2] - 'X');
        p2 += p2_choice_score + p2_outcome_score;
    }
    break :blk .{ p1, p2 };
};

pub fn main() !void {
    const stdout = std.io.getStdOut().writer();
    try stdout.print("Day2 (comptime only): P1: {}, P2: {}\n", answers);
}
