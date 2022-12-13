const std = @import("std");

pub fn main() !void {
    const file = try std.fs.cwd().openFile("src/input", .{});
    defer file.close();

    var buffered_reader = std.io.bufferedReader(file.reader());
    var reader = buffered_reader.reader();

    var top3 = [_]u32{0} ** 3;
    var accum: u32 = 0;
    var line_buf: [32]u8 = undefined;
    while (try reader.readUntilDelimiterOrEof(&line_buf, '\n')) |raw_line| {
        // Assume annoying CRLF on Windows.
        const line = if (@import("builtin").os.tag == .windows) raw_line[0 .. raw_line.len - 1] else raw_line;
        if (line.len == 0) {
            const i = std.mem.indexOfMin(u32, &top3);
            top3[i] = @max(top3[i], accum);
            accum = 0;
        } else accum += try std.fmt.parseInt(u32, line, 10);
    }
    const i = std.mem.indexOfMin(u32, &top3);
    top3[i] = @max(top3[i], accum);

    const p1 = std.mem.max(u32, &top3);
    const p2 = top3[0] + top3[1] + top3[2];
    const stdout = std.io.getStdOut().writer();
    try stdout.print("Day1 (runtime only): P1: {}, P2: {}\n", .{ p1, p2 });
}
