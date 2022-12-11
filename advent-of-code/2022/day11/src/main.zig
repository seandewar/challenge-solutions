const std = @import("std");

const parsed_input = blk: {
    @setEvalBranchQuota(10_000);
    const input = @embedFile("input");
    const Items = std.BoundedArray(u64, 64);
    var monkeys = std.BoundedArray(struct {
        items: Items,
        op: u8,
        operand: ?u8, // null == "old".
        div: u8, // assumed to always be a prime number.
        true_i: u8,
        false_i: u8,
    }, 16).init(0) catch unreachable;
    var max_worry: u64 = 1;
    var line_it = std.mem.tokenize(u8, input, std.cstr.line_sep);
    while (line_it.next()) |_| {
        var items = Items.init(0) catch unreachable;
        var items_it = std.mem.split(u8, line_it.next().?["  Starting items: ".len..], ", ");
        while (items_it.next()) |item_str| items.append(std.fmt.parseInt(u64, item_str, 10) catch unreachable) catch unreachable;
        const op_str = line_it.next().?["  Operation: new = old ".len..];
        const div = std.fmt.parseInt(u8, line_it.next().?["  Test: divisible by ".len..], 10) catch unreachable;
        monkeys.append(.{
            .items = items,
            .op = op_str[0],
            .operand = std.fmt.parseInt(u8, op_str[2..], 10) catch null,
            .div = div,
            .true_i = std.fmt.parseInt(u8, line_it.next().?["    If true: throw to monkey ".len..], 10) catch unreachable,
            .false_i = std.fmt.parseInt(u8, line_it.next().?["    If false: throw to monkey ".len..], 10) catch unreachable,
        }) catch unreachable;
        max_worry *= div;
    }
    break :blk .{ .monkeys = monkeys, .max_worry = max_worry };
};

fn computeAnswer(comptime p1: bool) usize {
    var monkeys = parsed_input.monkeys;
    var inspect_counts = [_]usize{0} ** parsed_input.monkeys.len;
    var round: u64 = 0;
    while (round < if (p1) 20 else 10_000) : (round += 1) {
        for (monkeys.slice()) |*monkey, i| {
            for (monkey.items.slice()) |item| {
                const operand = monkey.operand orelse item;
                const pre_worry = if (monkey.op == '*') item * operand else item + operand;
                const worry = if (p1) pre_worry / 3 else pre_worry % parsed_input.max_worry;
                const throw_i = if (worry % monkey.div == 0) monkey.true_i else monkey.false_i;
                monkeys.slice()[throw_i].items.append(worry) catch unreachable;
            }
            inspect_counts[i] += monkey.items.len;
            monkey.items.len = 0;
        }
    }
    var top2 = [_]usize{0} ** 2;
    for (inspect_counts) |count| {
        const min_i = std.mem.indexOfMin(usize, &top2);
        top2[min_i] = @max(top2[min_i], count);
    }
    return top2[0] * top2[1];
}

pub fn main() !void {
    const stdout = std.io.getStdOut().writer();
    const p1 = comptime blk: {
        @setEvalBranchQuota(100_000);
        break :blk computeAnswer(true);
    };
    const p2 = computeAnswer(false);
    try stdout.print("Day11 (P1 at comptime): P1: {}, P2: {}\n", .{ p1, p2 });
}
