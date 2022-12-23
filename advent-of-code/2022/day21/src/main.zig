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
        monkey.* = .{ .name = line[0..4] };
        if (std.ascii.isDigit(line[6]))
            monkey.number = std.fmt.parseInt(i64, line[6..], 10) catch unreachable
        else
            monkey.unparsed_op_line = line; // Resolve the operation and indices later.
    }
    std.sort.sort(Monkey, &monkeys, {}, struct {
        fn lessThan(context: void, lhs: Monkey, rhs: Monkey) bool {
            _ = context;
            return std.ascii.lessThanIgnoreCase(lhs.name, rhs.name);
        }
    }.lessThan);
    var root_i: ?usize = null;
    var humn_i: ?usize = null;
    for (monkeys) |*monkey, i| {
        if (root_i == null and std.mem.eql(u8, monkey.name, "root")) root_i = i;
        if (humn_i == null and std.mem.eql(u8, monkey.name, "humn")) humn_i = i;
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
    break :blk .{ .monkeys = monkeys, .root_i = root_i.?, .humn_i = humn_i.? };
};

fn computeAnswer(memo: *[parsed_input.monkeys.len]?i64, i: usize) ?i64 {
    if (memo[i]) |number| return number;
    if (parsed_input.monkeys[i].number) |number| {
        memo[i] = number;
        return number;
    }
    const lhs = computeAnswer(memo, parsed_input.monkeys[i].lhs_i) orelse return null;
    const rhs = computeAnswer(memo, parsed_input.monkeys[i].rhs_i) orelse return null;
    memo[i] = switch (parsed_input.monkeys[i].op) {
        '+' => lhs + rhs,
        '-' => lhs - rhs,
        '*' => lhs * rhs,
        '/' => std.math.divExact(i64, lhs, rhs) catch return null,
        else => unreachable,
    };
    return memo[i].?;
}
fn checkP2Diff(memo: *[parsed_input.monkeys.len]?i64, number: *i64, retry_step: i64) i64 {
    std.mem.set(?i64, memo, null);
    memo[parsed_input.humn_i] = number.*;
    _ = computeAnswer(memo, parsed_input.root_i) orelse {
        number.* += retry_step;
        return @call(.always_tail, checkP2Diff, .{ memo, number, retry_step });
    };
    return memo[parsed_input.monkeys[parsed_input.root_i].lhs_i].? - memo[parsed_input.monkeys[parsed_input.root_i].rhs_i].?;
}
pub fn main() !void {
    var memo = [_]?i64{null} ** parsed_input.monkeys.len;
    const stdout = std.io.getStdOut().writer();
    try stdout.print("Day21 (comptime parsing): P1: {}", .{computeAnswer(&memo, parsed_input.root_i).?});
    blk: { // Why solve when you can waste time bisecting instead? :P
        var l: i64 = 0;
        var l_diff = checkP2Diff(&memo, &l, 1);
        if (l_diff == 0) break :blk;
        var r: i64 = std.math.maxInt(i53);
        var r_diff = checkP2Diff(&memo, &r, -1);
        if (r_diff == 0) break :blk;
        while (l < r) {
            var m = l + @divTrunc(r - l, 2);
            const m_diff = checkP2Diff(&memo, &m, if (std.math.absCast(l_diff) < std.math.absCast(r_diff)) -1 else 1);
            if (m_diff == 0) break :blk;
            if ((l_diff < 0) != (m_diff < 0)) {
                r = m - 1;
                r_diff = checkP2Diff(&memo, &r, -1);
                if (r_diff == 0) break :blk;
            } else {
                l = m + 1;
                l_diff = checkP2Diff(&memo, &l, 1);
                if (l_diff == 0) break :blk;
            }
        }
    }
    try stdout.print(", P2: {}\n", .{memo[parsed_input.humn_i].?});
}
