const std = @import("std");

const parsed_input = blk: {
    @setEvalBranchQuota(10_000);
    const input = @embedFile("input");
    const ItemWorries = std.BoundedArray(u64, 64);
    var monkeys = std.BoundedArray(struct {
        item_worries: ItemWorries,
        op: u8,
        operand: ?u8, // null == "old".
        div: u8, // assumed to always be a prime number.
        true_i: u8,
        false_i: u8,
    }, 16).init(0) catch unreachable;
    var max_div: u64 = 1; // divisors are prime, so we only need to consider worry values less than this.
    var line_it = std.mem.tokenize(u8, input, std.cstr.line_sep);
    while (line_it.next()) |_| {
        var item_worries = ItemWorries.init(0) catch unreachable;
        var item_it = std.mem.split(u8, line_it.next().?["  Starting items: ".len..], ", ");
        while (item_it.next()) |str| item_worries.append(std.fmt.parseInt(u64, str, 10) catch unreachable) catch unreachable;
        const op_str = line_it.next().?["  Operation: new = old ".len..];
        const div = std.fmt.parseInt(u8, line_it.next().?["  Test: divisible by ".len..], 10) catch unreachable;
        max_div *= div;
        monkeys.append(.{
            .item_worries = item_worries,
            .op = op_str[0],
            .operand = std.fmt.parseInt(u8, op_str[2..], 10) catch null,
            .div = div,
            .true_i = std.fmt.parseInt(u8, line_it.next().?["    If true: throw to monkey ".len..], 10) catch unreachable,
            .false_i = std.fmt.parseInt(u8, line_it.next().?["    If false: throw to monkey ".len..], 10) catch unreachable,
        }) catch unreachable;
    }
    break :blk .{ .monkeys = monkeys, .max_div = max_div };
};

fn computeAnswer(comptime is_p1: bool) usize {
    var monkeys = parsed_input.monkeys;
    var inspects = [_]usize{0} ** parsed_input.monkeys.len;
    var round: u32 = 0;
    while (round < if (is_p1) 20 else 10_000) : (round += 1) {
        for (monkeys.slice()) |*monkey, i| {
            for (monkey.item_worries.slice()) |item_worry| {
                const operand = monkey.operand orelse item_worry;
                const op_worry = if (monkey.op == '*') item_worry * operand else item_worry + operand;
                const worry = if (is_p1) op_worry / 3 else op_worry % parsed_input.max_div;
                const throw_i = if (worry % monkey.div == 0) monkey.true_i else monkey.false_i;
                monkeys.slice()[throw_i].item_worries.append(worry) catch unreachable;
            }
            inspects[i] += monkey.item_worries.len;
            monkey.item_worries.len = 0;
        }
    }
    var top2 = [_]usize{0} ** 2;
    for (inspects) |count| {
        const min_i = std.mem.indexOfMin(usize, &top2);
        top2[min_i] = @max(top2[min_i], count);
    }
    return top2[0] * top2[1];
}
const p1 = blk: {
    @setEvalBranchQuota(100_000);
    break :blk computeAnswer(true);
};

pub fn main() !void {
    const stdout = std.io.getStdOut().writer();
    try stdout.print("Day11 (P1 at comptime): P1: {}, P2: {}\n", .{ p1, computeAnswer(false) });
}
