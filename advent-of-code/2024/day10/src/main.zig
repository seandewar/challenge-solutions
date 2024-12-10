const std = @import("std");

pub fn main() !void {
    var arena_instance: std.heap.ArenaAllocator = .init(std.heap.page_allocator);
    defer arena_instance.deinit();
    const arena = arena_instance.allocator();

    const input = try std.fs.cwd().readFileAlloc(arena, "src/input", std.math.maxInt(usize));
    const width = if (std.mem.indexOfScalar(u8, input, '\n')) |x| x + 1 else input.len;
    const height = std.math.divCeil(usize, input.len, @max(1, width)) catch unreachable;

    const p1_visited = try arena.alloc(bool, input.len);
    var p1: usize = 0;
    var p2: usize = 0;
    loop: for (0..height) |y| {
        for (0..width) |x| {
            const i = y * width + x;
            if (i >= input.len) break :loop;
            if (input[i] != '0') continue;
            @memset(p1_visited, false);
            p1 += trailheadScore(input, p1_visited, width, height, x, y, '0');
            p2 += trailheadScore(input, null, width, height, x, y, '0');
        }
    }

    try std.io.getStdOut().writer().print("Day10: P1: {}, P2: {}\n", .{ p1, p2 });
}

fn trailheadScore(input: []const u8, p1_visited: ?[]bool, width: usize, height: usize, x: usize, y: usize, want_c: u8) usize {
    const i = y * width + x;
    if (i >= input.len or input[i] != want_c) return 0;
    if (p1_visited) |visited| {
        if (visited[i]) return 0;
        visited[i] = true;
    }
    if (input[i] == '9') return 1;

    var ret: usize = 0;
    if (x < width - 1)
        ret += trailheadScore(input, p1_visited, width, height, x + 1, y, want_c + 1);
    if (x > 0)
        ret += trailheadScore(input, p1_visited, width, height, x - 1, y, want_c + 1);
    if (y < height - 1)
        ret += trailheadScore(input, p1_visited, width, height, x, y + 1, want_c + 1);
    if (y > 0)
        ret += trailheadScore(input, p1_visited, width, height, x, y - 1, want_c + 1);
    return ret;
}
