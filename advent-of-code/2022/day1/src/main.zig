const std = @import("std");

const elf_calories = blk: {
    @setEvalBranchQuota(500_000);
    const input = @embedFile("input");
    var result = [_]u32{0} ** (1 + std.mem.count(u8, input, "\n\n"));
    var line_it = std.mem.split(u8, input, "\n");
    var i: usize = 0;
    while (line_it.next()) |line| {
        if (line.len == 0) {
            i += 1;
            continue;
        }
        result[i] += std.fmt.parseInt(u32, line, 10) catch unreachable;
    }

    break :blk result;
};

const top_elves = blk: {
    @setEvalBranchQuota(10_000);
    var result: [elf_calories.len]usize = std.simd.iota(usize, elf_calories.len);
    std.sort.sort(usize, &result, {}, struct {
        fn compare(context: void, i: usize, j: usize) bool {
            _ = context;
            return elf_calories[i] > elf_calories[j];
        }
    }.compare);

    break :blk result;
};

const p1 = elf_calories[top_elves[0]];
const p2 = p1 + elf_calories[top_elves[1]] + elf_calories[top_elves[2]];

pub fn main() !void {
    const stdout = std.io.getStdOut().writer();
    try stdout.print("Day1 (comptime only): P1: {}, P2: {}\n", .{ p1, p2 });
}
