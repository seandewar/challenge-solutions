const std = @import("std");

fn isOrdered(lines: [2][]const u8) ?bool {
    var pair = lines;
    while (pair[0].len != 0 and pair[1].len != 0) {
        if (pair[0][0] == '[' or pair[1][0] == '[') { // List vs List or List vs Integer.
            var inner_pair = pair;
            for (inner_pair) |*packet, pair_i| {
                if (packet.*[0] != '[') {
                    pair[pair_i] = if (std.mem.indexOfScalar(u8, packet.*, ',')) |len| blk: {
                        packet.len = len;
                        break :blk pair[pair_i][len..];
                    } else "";
                    continue;
                }
                var len: usize = 0;
                var depth: u8 = 0;
                for (packet.*[1..]) |c| {
                    if (c == '[') depth += 1;
                    if (c == ']' and depth == 0) break;
                    if (c == ']') depth -= 1;
                    len += 1;
                }
                packet.* = packet.*[1 .. 1 + len];
                pair[pair_i] = pair[pair_i][2 + len ..];
            }
            if (isOrdered(inner_pair)) |ordered| return ordered;
        } else { // Integer vs Integer.
            var ints: [2]u8 = undefined;
            for (pair) |*packet, pair_i| {
                const str = if (std.mem.indexOfScalar(u8, packet.*, ',')) |len| packet.*[0..len] else packet.*;
                ints[pair_i] = std.fmt.parseInt(u8, str, 10) catch unreachable;
                packet.* = packet.*[str.len..];
            }
            if (ints[0] < ints[1]) return true;
            if (ints[0] > ints[1]) return false;
        }
        for (pair) |*packet| { // Skip trailing ','s, if any.
            if (packet.len != 0 and packet.*[0] == ',') packet.* = packet.*[1..];
        }
    }
    if (pair[0].len == 0 and pair[1].len == 0) return null; // Inconclusive.
    return pair[0].len == 0;
}

fn lessThan(context: void, lhs: []const u8, rhs: []const u8) bool {
    _ = context;
    return isOrdered(.{ lhs[1 .. lhs.len - 1], rhs[1 .. rhs.len - 1] }).?;
}
const answers = blk: {
    @setEvalBranchQuota(1_000_000);
    const input = @embedFile("input");
    var packet_count = 2; // Make room for decoder packets.
    var line_it = std.mem.tokenize(u8, input, std.cstr.line_sep);
    while (line_it.next()) |_| packet_count += 1;
    var packets = std.BoundedArray([]const u8, packet_count).init(0) catch unreachable;
    packets.appendSlice(&.{ "[[2]]", "[[6]]" }) catch unreachable;
    var p1 = 0;
    var pair_i = 1;
    line_it = std.mem.tokenize(u8, input, std.cstr.line_sep);
    while (line_it.next()) |lhs| : (pair_i += 1) {
        const rhs = line_it.next().?;
        packets.appendSlice(&.{ lhs, rhs }) catch unreachable;
        if (lessThan({}, lhs, rhs)) p1 += pair_i;
    }
    std.sort.sort([]const u8, packets.slice(), {}, lessThan);
    var p2 = 1;
    for (packets.slice()) |packet, i| {
        if (std.mem.eql(u8, packet, "[[2]]") or std.mem.eql(u8, packet, "[[6]]")) p2 *= i + 1;
    }
    break :blk .{ p1, p2 };
};

pub fn main() !void {
    const stdout = std.io.getStdOut().writer();
    try stdout.print("Day13 (comptime only): P1: {}, P2: {}\n", answers);
}
