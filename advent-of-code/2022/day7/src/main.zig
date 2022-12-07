const std = @import("std");

const Node = struct {
    const RefBuffer = std.BoundedArray(*@This(), 64);
    name: []const u8,
    size: u32 = 0,
    children: RefBuffer = RefBuffer.init(0) catch unreachable,
};
const node_buf = blk: {
    @setEvalBranchQuota(1_000_000);
    const input = @embedFile("input");
    var buf = std.BoundedArray(Node, 1024).init(1) catch unreachable;
    buf.slice()[0] = .{ .name = "" };
    var path = Node.RefBuffer.init(1) catch unreachable;
    path.slice()[0] = &buf.slice()[0];
    var line_it = std.mem.tokenize(u8, input, "\n");
    _ = line_it.next().?; // Skip "$ cd /" (we already start in the root directory).
    while (line_it.next()) |line| {
        const cwd = path.get(path.len - 1);
        if (std.mem.startsWith(u8, line, "dir ")) { // listed directory
            buf.append(.{ .name = line[4..] }) catch unreachable;
            cwd.children.append(&buf.slice()[buf.len - 1]) catch unreachable;
        } else if (std.ascii.isDigit(line[0])) { // listed file
            const size = std.fmt.parseInt(u32, line[0..std.mem.indexOfScalar(u8, line, ' ').?], 10) catch unreachable;
            for (path.slice()) |node| node.size += size;
        } else if (std.mem.eql(u8, line, "$ cd ..")) { // up a directory
            _ = path.pop();
        } else if (std.mem.startsWith(u8, line, "$ cd ")) { // down a directory
            for (cwd.children.slice()) |node| {
                if (std.mem.eql(u8, node.name, line[5..])) {
                    path.append(node) catch unreachable;
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
