const std = @import("std");

fn lessThan(lhs: []const u8, rhs: []const u8) ?bool {
    var pair = [_][]const u8{ lhs, rhs };
    while (pair[0].len != 0 and pair[1].len != 0) {
        if (pair[0][0] == '[' or pair[1][0] == '[') { // List vs List or List vs Integer.
            var inner_pair = pair;
            for (inner_pair) |*packet, pair_i| {
                if (packet.*[0] != '[') { // Integer.
                    if (std.mem.indexOfScalar(u8, packet.*, ',')) |len| packet.len = len;
                    continue;
                }
                var depth: usize = 0;
                var len: usize = 0;
                for (packet.*[1..]) |c| {
                    if (c == '[') depth += 1;
                    if (c == ']' and depth == 0) break;
                    if (c == ']') depth -= 1;
                    len += 1;
                }
                packet.* = packet.*[1 .. 1 + len];
                pair[pair_i] = pair[pair_i][2 + len ..];
            }
            if (lessThan(inner_pair[0], inner_pair[1])) |lt| return lt;
        } else { // Integer vs Integer.
            var ints: [2]u8 = undefined;
            for (pair) |*packet, pair_i| {
                const str = if (std.mem.indexOfScalar(u8, packet.*, ',')) |len| packet.*[0..len] else packet.*;
                ints[pair_i] = std.fmt.parseInt(u8, str, 10) catch unreachable;
            }
            if (ints[0] != ints[1]) return ints[0] < ints[1];
        }
        for (pair) |*packet| packet.* = if (std.mem.indexOfScalar(u8, packet.*, ',')) |i| packet.*[i + 1 ..] else "";
    }
    return if (pair[0].len != 0 or pair[1].len != 0) pair[0].len == 0 else null;
}
const answers = blk: {
    @setEvalBranchQuota(1_000_000);
    const input = @embedFile("input");
    var p1 = 0;
    var p2_decoder_i = [_]usize{ 1, 2 };
    var pair_i = 1;
    var line_it = std.mem.tokenize(u8, input, std.cstr.line_sep);
    while (line_it.next()) |lhs| : (pair_i += 1) {
        const rhs = line_it.next().?;
        if (lessThan(lhs, rhs).?) p1 += pair_i;
        p2_decoder_i[0] += @boolToInt(lessThan(lhs, "[[2]]").?) + @boolToInt(lessThan(rhs, "[[2]]").?);
        p2_decoder_i[1] += @boolToInt(lessThan(lhs, "[[6]]").?) + @boolToInt(lessThan(rhs, "[[6]]").?);
    }
    break :blk .{ p1, p2_decoder_i[0] * p2_decoder_i[1] };
};

pub fn main() !void {
    const stdout = std.io.getStdOut().writer();
    try stdout.print("Day13 (comptime only): P1: {}, P2: {}\n", answers);
}
