const std = @import("std");

const answers = blk: {
    @setEvalBranchQuota(100_000);
    const input = @embedFile("input");
    var line_it = std.mem.tokenize(u8, input, "\n");
    var p2_seen = [_]u3{0} ** (1 + 'z' - 'A');
    var p2_elf_i = 0;
    var p1_answer: u32 = 0;
    var p2_answer: u32 = 0;
    while (line_it.next()) |line| {
        var p1_seen = [_]bool{false} ** (1 + 'z' - 'A');
        for (line[0 .. line.len / 2]) |item| p1_seen[item - 'A'] = true;
        for (line[line.len / 2 ..]) |item| {
            if (p1_seen[item - 'A']) {
                p1_answer += if (std.ascii.isLower(item)) item - 'a' + 1 else item - 'A' + 27;
                break;
            }
        }
        for (line) |item| {
            p2_seen[item - 'A'] |= 1 << p2_elf_i;
            if (p2_seen[item - 'A'] == 0b111) {
                p2_answer += if (std.ascii.isLower(item)) item - 'a' + 1 else item - 'A' + 27;
                std.mem.set(u3, &p2_seen, 0);
                break;
            }
        }
        p2_elf_i = (p2_elf_i + 1) % 3;
    }
    break :blk .{ p1_answer, p2_answer };
};

pub fn main() !void {
    const stdout = std.io.getStdOut().writer();
    try stdout.print("Day3 (comptime only): P1: {}, P2: {}\n", answers);
}
