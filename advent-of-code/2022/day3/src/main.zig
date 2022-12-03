const std = @import("std");

const input = @embedFile("input");

const p1 = blk: {
    @setEvalBranchQuota(100_000);
    var line_it = std.mem.tokenize(u8, input, "\n");
    var result: u32 = 0;
    while (line_it.next()) |line| {
        var seen = [_]bool{false} ** (1 + 'z' - 'A');
        for (line[0 .. line.len / 2]) |item| seen[item - 'A'] = true;
        for (line[line.len / 2 ..]) |item| {
            if (seen[item - 'A']) {
                result += if (std.ascii.isLower(item)) item - 'a' + 1 else item - 'A' + 27;
                break;
            }
        }
    }
    break :blk result;
};

const p2 = blk: {
    @setEvalBranchQuota(100_000);
    var line_it = std.mem.tokenize(u8, input, "\n");
    var seen = [_]u3{0} ** (1 + 'z' - 'A');
    var elf_i = 0;
    var result: u32 = 0;
    while (line_it.next()) |line| {
        for (line) |item| {
            seen[item - 'A'] |= 1 << elf_i;
            if (seen[item - 'A'] == 0b111) {
                result += if (std.ascii.isLower(item)) item - 'a' + 1 else item - 'A' + 27;
                std.mem.set(u3, &seen, 0);
                break;
            }
        }
        elf_i = (elf_i + 1) % 3;
    }
    break :blk result;
};

pub fn main() !void {
    const stdout = std.io.getStdOut().writer();
    try stdout.print("Day3 (comptime only): P1: {}, P2: {}\n", .{ p1, p2 });
}
