const std = @import("std");

const parsed_input = blk: {
    @setEvalBranchQuota(1_000_000);
    const input = @embedFile("input");
    var line_it = std.mem.tokenize(u8, input, std.cstr.line_sep);
    const Stack = std.BoundedArray(u8, 9 * 8);
    var stacks = [_]Stack{Stack.init(0) catch unreachable} ** 9;
    // Parse the diagram (first 8 lines).
    var row_i = 0;
    while (row_i < 8) : (row_i += 1) {
        const line = line_it.next().?;
        var crate_i = 0;
        // Skip the first "[" (1), and the next "X] [" (4*i) with each iteration.
        while (1 + 4 * crate_i < line.len) : (crate_i += 1) {
            const crate = line[1 + 4 * crate_i];
            if (crate == ' ') continue;
            stacks[crate_i].append(crate) catch unreachable;
        }
    }
    // Stacks were parsed in reverse order.
    for (stacks) |*stack| std.mem.reverse(u8, stack.slice());
    _ = line_it.next(); // Skip the empty line.
    // Parse the move commands.
    var cmd_count = 0;
    var cmd_count_it = line_it;
    while (cmd_count_it.next()) |_| cmd_count += 1;
    var cmds: [cmd_count]struct { count: u8, from_i: u8, to_i: u8 } = undefined;
    for (cmds) |*cmd| {
        var cmd_it = std.mem.tokenize(u8, line_it.next().?, "move frt");
        const count = std.fmt.parseInt(u8, cmd_it.next().?, 10) catch unreachable;
        const from = std.fmt.parseInt(u8, cmd_it.next().?, 10) catch unreachable;
        const to = std.fmt.parseInt(u8, cmd_it.next().?, 10) catch unreachable;
        cmd.* = .{ .count = count, .from_i = from - 1, .to_i = to - 1 };
    }
    break :blk .{ .stacks = stacks, .cmds = cmds };
};

fn computeAnswer(reverse_move: bool) [9]u8 {
    var stacks = parsed_input.stacks;
    for (parsed_input.cmds) |cmd| {
        const from = &stacks[cmd.from_i];
        const to = &stacks[cmd.to_i];
        const move = from.slice()[from.len - cmd.count ..];
        if (reverse_move) std.mem.reverse(u8, move);
        to.appendSlice(move) catch unreachable;
        from.len -= move.len;
    }
    var result: [9]u8 = undefined;
    for (result) |*top, i| top.* = stacks[i].get(stacks[i].len - 1);
    return result;
}
const answers = blk: {
    @setEvalBranchQuota(100_000);
    break :blk .{ computeAnswer(true), computeAnswer(false) };
};

pub fn main() !void {
    const stdout = std.io.getStdOut().writer();
    try stdout.print("Day5 (comptime only): P1: {s}, P2: {s}\n", answers);
}
