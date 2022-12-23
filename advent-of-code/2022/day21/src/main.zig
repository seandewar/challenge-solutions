const std = @import("std");
const Monkey = struct {
    name: []const u8,
    number: ?i64 = null, // Union crashes stage2, so check for null to tell if this is a number or op monkey.
    lhs_i: usize = undefined,
    rhs_i: usize = undefined,
    op: u8 = undefined,
    unparsed_op_line: []const u8 = undefined,
};
const parsed_input = blk: {
    @setEvalBranchQuota(1_000_000);
    const input = @embedFile("input");
    var monkey_count = 0;
    var line_it = std.mem.tokenize(u8, input, std.cstr.line_sep);
    while (line_it.next()) |_| monkey_count += 1;
    var monkeys: [monkey_count]Monkey = undefined;
    line_it.reset();
    for (monkeys) |*monkey| {
        const line = line_it.next().?;
        if (std.ascii.isDigit(line[6])) {
            monkey.* = .{ .name = line[0..4], .number = std.fmt.parseInt(i64, line[6..], 10) catch unreachable };
        } else {
            monkey.* = .{ .name = line[0..4], .unparsed_op_line = line }; // Resolve the operation and indices later.
        }
    }
    std.sort.sort(Monkey, &monkeys, {}, struct {
        fn lessThan(context: void, lhs: Monkey, rhs: Monkey) bool {
            _ = context;
            return std.ascii.lessThanIgnoreCase(lhs.name, rhs.name);
        }
    }.lessThan);
    var root_i: ?usize = null;
    for (monkeys) |*monkey, i| {
        if (root_i == null and std.mem.eql(u8, monkey.name, "root")) root_i = i;
        if (monkey.number != null) continue;
        const line = monkey.unparsed_op_line;
        const compareFn = struct {
            fn compareFn(context: void, lhs: Monkey, rhs: Monkey) std.math.Order {
                _ = context;
                return std.ascii.orderIgnoreCase(lhs.name, rhs.name);
            }
        }.compareFn;
        monkey.lhs_i = std.sort.binarySearch(Monkey, .{ .name = line[6..10] }, &monkeys, {}, compareFn).?;
        monkey.rhs_i = std.sort.binarySearch(Monkey, .{ .name = line[13..] }, &monkeys, {}, compareFn).?;
        monkey.op = line[11];
    }
    break :blk .{ .monkeys = monkeys, .root_i = root_i.? };
};

fn computeAnswer(memo: *[parsed_input.monkeys.len]?i64, i: usize) i64 {
    if (parsed_input.monkeys[i].number) |number| return number;
    if (memo[i]) |number| return number;
    const lhs = computeAnswer(memo, parsed_input.monkeys[i].lhs_i);
    const rhs = computeAnswer(memo, parsed_input.monkeys[i].rhs_i);
    memo[i] = switch (parsed_input.monkeys[i].op) {
        '+' => lhs + rhs,
        '-' => lhs - rhs,
        '*' => lhs * rhs,
        '/' => @divExact(lhs, rhs),
        else => unreachable,
    };
    return memo[i].?;
}
pub fn main() !void {
    var memo = [_]?i64{null} ** parsed_input.monkeys.len;
    const stdout = std.io.getStdOut().writer();
    try stdout.print("Day21 (comptime parsing): P1: {}, P2: {}\n", .{ computeAnswer(&memo, parsed_input.root_i), 0 });
}
