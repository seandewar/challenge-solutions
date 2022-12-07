const std = @import("std");

const node_buf = blk: {
    const Node = struct {
        const Children = std.BoundedArray(*@This(), 64);
        name: []const u8,
        size: u32 = 0,
        parent: ?*@This(),
        children: Children = Children.init(0) catch unreachable,
    };
    @setEvalBranchQuota(1_000_000);
    const input = @embedFile("input");
    var buf = std.BoundedArray(Node, 1024).init(0) catch unreachable;
    buf.append(.{ .name = "", .parent = null }) catch unreachable;
    var cwd = &buf.slice()[0];
    var line_it = std.mem.tokenize(u8, input, "\n");
    _ = line_it.next().?; // Skip "$ cd /" (we already start in the root directory).
    while (line_it.next()) |line| {
        if (std.mem.startsWith(u8, line, "dir ")) { // ls child directory
            buf.append(.{ .name = line["dir ".len..], .parent = cwd }) catch unreachable;
            cwd.children.append(&buf.slice()[buf.len - 1]) catch unreachable;
        } else if (std.ascii.isDigit(line[0])) { // ls child file
            const slice = line[0..std.mem.indexOfScalar(u8, line, ' ').?];
            const size = std.fmt.parseInt(u32, slice, 10) catch unreachable;
            var dir_node: ?*Node = cwd;
            while (dir_node) |dir| {
                dir.size += size;
                dir_node = dir.parent;
            }
        } else if (std.mem.startsWith(u8, line, "$ cd ")) { // change directory
            const dir = line["$ cd ".len..];
            if (std.mem.eql(u8, dir, "..")) {
                cwd = cwd.parent.?;
            } else for (cwd.children.slice()) |child| {
                if (std.mem.eql(u8, child.name, dir)) {
                    cwd = child;
                    break;
                }
            }
        }
    }
    break :blk buf;
};

const answers = blk: {
    var p1 = 0;
    var p2 = node_buf.get(0).size;
    const p2_needed_space = 30_000_000 - (70_000_000 - node_buf.get(0).size);
    for (node_buf.slice()) |node| {
        if (node.size <= 100_000) p1 += node.size;
        if (node.size >= p2_needed_space) p2 = @min(p2, node.size);
    }
    break :blk .{ p1, p2 };
};

pub fn main() !void {
    const stdout = std.io.getStdOut().writer();
    try stdout.print("Day7 (comptime only): P1: {}, P2: {}\n", answers);
}
