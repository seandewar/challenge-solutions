const std = @import("std");

const parsed_input = blk: {
    @setEvalBranchQuota(1_000_000);
    const input = @embedFile("input");
    var line_it = std.mem.tokenize(u8, input, "\n");
    var stack_bufs: [9][9 * 8]u8 = undefined;
    var stack_lens = [_]u8{0} ** stack_bufs.len;
    // Parse the diagram (first 8 lines).
    var row_i = 0;
    while (row_i < 8) : (row_i += 1) {
        const line = line_it.next().?;
        var crate_i = 0;
        // Skip the first "[" (1), and the next "X] [" (4*i) with each iteration.
        while (1 + 4 * crate_i < line.len) : (crate_i += 1) {
            const crate = line[1 + 4 * crate_i];
            if (crate == ' ') continue;
            stack_bufs[crate_i][stack_lens[crate_i]] = crate;
            stack_lens[crate_i] += 1;
        }
    }
    // Stacks were parsed in reverse order.
    for (stack_bufs) |*buf, i| std.mem.reverse(u8, buf[0..stack_lens[i]]);
    _ = line_it.next(); // Skip the empty line.
    // Parse the move commands.
    var cmd_count = 0;
    var cmd_count_it = line_it;
    while (cmd_count_it.next()) |_| cmd_count += 1;
    var cmds: [cmd_count]struct { count: u8, from_i: u8, to_i: u8 } = undefined;
    for (cmds) |*cmd| {
        var cmd_it = std.mem.split(u8, line_it.next().?, " ");
        _ = cmd_it.next().?; // Skip "move".
        const count = std.fmt.parseInt(u8, cmd_it.next().?, 10) catch unreachable;
        _ = cmd_it.next().?; // Skip "from".
        const from = std.fmt.parseInt(u8, cmd_it.next().?, 10) catch unreachable;
        _ = cmd_it.next().?; // Skip "to".
        const to = std.fmt.parseInt(u8, cmd_it.next().?, 10) catch unreachable;
        cmd.* = .{ .count = count, .from_i = from - 1, .to_i = to - 1 };
    }
    break :blk .{ .stack_bufs = stack_bufs, .stack_lens = stack_lens, .cmds = cmds };
};

fn computeAnswer(reverse: bool) [9]u8 {
    var stack_bufs = parsed_input.stack_bufs;
    var stacks: [9][]u8 = undefined;
    for (stacks) |*stack, i| stack.* = stack_bufs[i][0..parsed_input.stack_lens[i]];
    for (parsed_input.cmds) |*cmd| {
        const from = &stacks[cmd.from_i];
        const to = &stacks[cmd.to_i];
        const moved = from.*[from.len - cmd.count ..];
        to.len += moved.len;
        var i = moved.len;
        while (i != 0) : (i -= 1)
            to.*[to.len - i] = moved[if (reverse) i - 1 else moved.len - i];
        from.len -= moved.len;
    }
    var result: [9]u8 = undefined;
    for (result) |*top, i| top.* = stacks[i][stacks[i].len - 1];
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
