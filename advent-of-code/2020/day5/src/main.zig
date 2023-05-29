const std = @import("std");

pub fn main() !void {
    var seen_seats = [_]bool{false} ** 1024;
    var max_id: u10 = 0;

    var line_it = std.mem.tokenize(u8, @embedFile("input"), std.cstr.line_sep);
    while (line_it.next()) |line| {
        std.debug.assert(line.len == 10);

        var f: u8 = 0;
        var b: u8 = 128;
        for (line[0..7]) |c| {
            const m = f + (b - f) / 2;
            switch (c) {
                'F' => b = m,
                'B' => f = m,
                else => unreachable,
            }
        }

        var l: u4 = 0;
        var r: u4 = 8;
        for (line[7..]) |c| {
            const m = l + (r - l) / 2;
            switch (c) {
                'L' => r = m,
                'R' => l = m,
                else => unreachable,
            }
        }

        const id = @as(u10, f) * 8 + l;
        max_id = @max(max_id, id);
        seen_seats[id] = true;
    }

    const start_id = std.mem.indexOfScalar(bool, &seen_seats, true).?;
    const my_id = std.mem.indexOfScalarPos(bool, &seen_seats, start_id + 1, false).?;
    try std.io.getStdOut().writer().print("D5: P1: {d}, P2: {d}\n", .{ max_id, my_id });
}
