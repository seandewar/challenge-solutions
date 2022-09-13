///! Pretty much just a 2-sum / 3-sum problem.
const std = @import("std");

const input = blk: {
    @setEvalBranchQuota(20_000);
    const in = @embedFile("input.txt");

    var iter = std.mem.tokenize(u8, in, "\n");
    var len = 0;
    while (iter.next() != null) {
        len += 1;
    }

    var buf: [len]u16 = undefined;
    iter = std.mem.tokenize(u8, in, "\n");
    var i = 0;
    while (i < len) : (i += 1) {
        const line = iter.next().?;
        buf[i] = std.fmt.parseUnsigned(u16, line, 10) catch unreachable;
    }

    std.sort.sort(u16, &buf, {}, std.sort.asc(u16));
    break :blk buf;
};

pub fn main() !void {
    const stdout = std.io.getStdOut().writer();
    try stdout.print("Day 1\n", .{});

    var arena = std.heap.ArenaAllocator.init(std.heap.page_allocator);
    defer arena.deinit();
    var map = std.hash_map.AutoHashMap(u16, u8).init(arena.allocator());
    defer map.deinit();
    for (input) |num| {
        var result = try map.getOrPutValue(num, 0);
        result.value_ptr.* += 1;
    }

    // Part 1
    for (input) |num| {
        const target = 2020 - num;
        if (map.get(target)) |count| {
            if (num != target or count > 1) {
                try stdout.print("P1: {d}\n", .{@as(u32, num) * target});
                break;
            }
        }
    }

    // Part 2
    var i: u8 = 0;
    while (i < input.len - 2) : (i += 1) {
        var j: u8 = i + 1;
        while (j < input.len - 1) : (j += 1) {
            if (input[i] + input[j] > 2020) {
                break;
            }
            const target = 2020 - input[i] - input[j];
            if (map.get(target)) |count| {
                var needed_count: u8 = 1;
                if (input[i] == target) needed_count += 1;
                if (input[j] == target) needed_count += 1;
                if (count >= needed_count) {
                    try stdout.print("P2: {d}\n", .{@as(u64, input[i]) * input[j] * target});
                    return;
                }
            }
        }
    }
}
