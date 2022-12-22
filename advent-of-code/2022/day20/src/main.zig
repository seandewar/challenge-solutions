const std = @import("std");
const List = std.TailQueue(i64);
const parsed_input = blk: {
    @setEvalBranchQuota(1_000_000);
    const input = @embedFile("input");
    var count = 0;
    var line_it = std.mem.tokenize(u8, input, std.cstr.line_sep);
    while (line_it.next()) |_| count += 1;
    var nodes: [count]List.Node = undefined;
    var zero_i: usize = undefined;
    line_it.reset();
    for (nodes) |*node, i| {
        node.* = .{ .data = std.fmt.parseInt(i64, line_it.next().?, 10) catch unreachable };
        if (node.data == 0) zero_i = i;
    }
    break :blk .{ .nodes = nodes, .zero_i = zero_i };
};

fn computeAnswer(comptime p1: bool) i64 {
    var nodes = parsed_input.nodes;
    var list = List{};
    for (nodes) |*node| {
        if (!p1) node.data *= 811589153;
        list.append(node);
    }
    var i: usize = 0;
    while (i < if (p1) 1 else 10) : (i += 1) {
        for (nodes) |*node| {
            var node2 = node;
            var rem_moves = @rem(node.data, @intCast(i64, nodes.len - 1)); // -1 to discount the number itself we're moving.
            while (rem_moves != 0) : (rem_moves += if (rem_moves < 0) 1 else -1) {
                node2 = if (rem_moves < 0)
                    node2.prev orelse list.last.?
                else
                    node2.next orelse list.first.?;
            }
            if (node == node2) continue;
            list.remove(node);
            if (node.data < 0) list.insertBefore(node2, node) else list.insertAfter(node2, node);
        }
    }
    var result: i64 = 0;
    var node = nodes[parsed_input.zero_i].next orelse list.first.?;
    i = 1;
    while (i <= 3000) : (i += 1) {
        if (i % 1000 == 0) result += node.data;
        node = node.next orelse list.first.?;
    }
    return result;
}
pub fn main() !void {
    const stdout = std.io.getStdOut().writer();
    try stdout.print("Day20 (comptime parsing): P1: {}, P2: {}\n", .{ computeAnswer(true), computeAnswer(false) });
}
