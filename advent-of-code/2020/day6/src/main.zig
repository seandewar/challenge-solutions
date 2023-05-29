const std = @import("std");

pub fn main() !void {
    var p1: usize = 0;
    var p2: usize = 0;

    const Set = std.bit_set.IntegerBitSet(26);
    var answered = Set.initEmpty();
    var all_answered = Set.initEmpty();

    var line_it = std.mem.split(u8, @embedFile("input"), std.cstr.line_sep);
    while (line_it.next()) |line| {
        if (line.len == 0) {
            p1 += answered.count();
            p2 += all_answered.count();
            answered.mask = 0;
            all_answered.mask = 0;
            continue;
        }

        var this_answered = Set.initEmpty();
        for (line) |c| this_answered.set(c - 'a');

        if (answered.mask == 0) {
            all_answered = this_answered;
        } else {
            all_answered.setIntersection(this_answered);
        }
        answered.setUnion(this_answered);
    }
    p1 += answered.count();
    p2 += all_answered.count();

    try std.io.getStdOut().writer().print("D6: P1: {}, P2: {}\n", .{ p1, p2 });
}
