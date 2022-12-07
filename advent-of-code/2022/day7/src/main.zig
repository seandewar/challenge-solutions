const std = @import("std");

const node_buf = blk: {
    const Node = struct {
        name: []const u8,
        size: u32 = 0,
        parent_i: ?usize,
        child_indices: std.BoundedArray(usize, 64) = std.BoundedArray(usize, 64).init(0) catch unreachable,
    };
    @setEvalBranchQuota(1_000_000);
    const input = @embedFile("input");
    var buf = std.BoundedArray(Node, 1024).init(0) catch unreachable;
    buf.append(.{ .name = "", .parent_i = null }) catch unreachable;
    var cwd_i: usize = 0;
    var line_it = std.mem.tokenize(u8, input, "\n");
    _ = line_it.next().?; // Skip "$ cd /" (we already start in the root directory).
    while (line_it.next()) |line| {
        if (std.mem.startsWith(u8, line, "dir ")) { // ls child directory
            buf.append(.{ .name = line["dir ".len..], .parent_i = cwd_i }) catch unreachable;
            buf.slice()[cwd_i].child_indices.append(buf.len - 1) catch unreachable;
        } else if (std.ascii.isDigit(line[0])) { // ls child file
            const slice = line[0..std.mem.indexOfScalar(u8, line, ' ').?];
            const size = std.fmt.parseInt(u32, slice, 10) catch unreachable;
            var i = cwd_i;
            while (true) {
                buf.slice()[i].size += size;
                if (buf.get(i).parent_i) |parent_i| i = parent_i else break;
            }
        } else if (std.mem.startsWith(u8, line, "$ cd ")) { // change directory
            const dir = line["$ cd ".len..];
            if (std.mem.eql(u8, dir, "..")) {
                cwd_i = buf.get(cwd_i).parent_i.?;
            } else for (buf.get(cwd_i).child_indices.slice()) |i| {
                if (std.mem.eql(u8, buf.get(i).name, dir)) {
                    cwd_i = i;
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
