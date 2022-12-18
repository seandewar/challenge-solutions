const std = @import("std");
const shapes = [_]u16{
    0b0000_0000_0000_1111,
    0b0000_0100_1110_0100,
    0b0000_0010_0010_1110,
    0b1000_1000_1000_1000,
    0b0000_0000_1100_1100,
};
const cmds = blk: { // Just consider the first line.
    @setEvalBranchQuota(100_000);
    const input = @embedFile("input");
    break :blk input[0 .. std.mem.indexOf(u8, input, std.cstr.line_sep) orelse input.len];
};

fn shapeDo(grid: [][7]bool, floor_y: usize, shape_i: usize, shape_x: u8, shape_y: usize, comptime action: enum {
    validateX,
    validateY,
    rest,
}) union(enum) { is_valid: bool, max_y: usize } {
    var i: u8 = 0;
    var max_y: usize = 0;
    var shape = shapes[shape_i % shapes.len];
    while (shape != 0) : ({
        i += 1;
        shape >>= 1;
    }) {
        if (shape & 1 == 0) continue;
        const x = shape_x + 3 - (i % 4);
        const y = shape_y + i / 4;
        max_y = @max(max_y, y);
        switch (action) {
            .validateX => if (x >= 7 or grid[y - 1 - floor_y][x]) break,
            .validateY => if (y <= floor_y or grid[y - 1 - floor_y][x]) break,
            .rest => grid[y - 1 - floor_y][x] = true,
        }
    }
    return switch (action) {
        .validateX, .validateY => .{ .is_valid = shape == 0 },
        .rest => .{ .max_y = max_y },
    };
}
pub fn main() !void {
    const stdout = std.io.getStdOut().writer();
    var grid = [_][7]bool{.{false} ** 7} ** 8192; // Index 0 is the bottom-most point of the grid.
    var floor_y: usize = 0;
    var max_y: usize = 0;
    var shape_i: usize = 0;
    var shape_x: u8 = 2; // Bottom-left of the shape.
    var shape_y: usize = 4;
    var cmd_i: usize = 0;
    var repeat_start: ?struct { cmd_i: usize, shape_i: usize, max_y: usize } = null;
    while (shape_i < 1_000_000_000_000) {
        const new_shape_x = if (cmds[cmd_i] == '<') shape_x -| 1 else shape_x + 1;
        cmd_i = (cmd_i + 1) % cmds.len;
        if (new_shape_x != shape_x and shapeDo(&grid, floor_y, shape_i, new_shape_x, shape_y, .validateX).is_valid)
            shape_x = new_shape_x;
        if (!shapeDo(&grid, floor_y, shape_i, shape_x, shape_y - 1, .validateY).is_valid) {
            max_y = @max(max_y, shapeDo(&grid, floor_y, shape_i, shape_x, shape_y, .rest).max_y);
            // If a new 7-block-wide floor is formed by this piece, blocks under the floor are inaccessible and can be forgotten.
            // The pattern may also be repeating if we're processing the same part of the input as a previous time a floor was formed.
            var rev_it = std.mem.reverseIterator(grid[shape_y - 1 - floor_y .. shape_y - 1 - floor_y + 4]);
            while (rev_it.next()) |row| {
                if (!std.mem.allEqual(bool, &row, true)) continue;
                const new_floor_y = shape_y + rev_it.index;
                std.mem.copy([7]bool, &grid, grid[new_floor_y - floor_y .. max_y - floor_y]);
                std.mem.set([7]bool, grid[max_y - new_floor_y .. max_y - floor_y], .{false} ** 7);
                floor_y = new_floor_y;
                if (floor_y == max_y) { // Easy to detect a repeating floor with no blocks above it.
                    if (repeat_start == null) {
                        repeat_start = .{ .cmd_i = cmd_i, .shape_i = shape_i, .max_y = max_y };
                    } else if (shape_i >= 2022 and repeat_start.?.cmd_i == cmd_i) { // Part of grid is repeating now; skip ahead.
                        const shape_count = shape_i - repeat_start.?.shape_i;
                        const mul = (1_000_000_000_000 - 1 - shape_i) / shape_count;
                        shape_i += shape_count * mul;
                        max_y += (max_y - repeat_start.?.max_y) * mul;
                        floor_y = max_y;
                    }
                }
                break;
            }
            shape_i += 1;
            shape_x = 2;
            shape_y = max_y + 4;
            if (shape_i == 2022) try stdout.print("Day17 (comptime parsing): P1: {}", .{max_y});
        } else shape_y -= 1;
    }
    try stdout.print(", P2: {}\n", .{max_y});
}
