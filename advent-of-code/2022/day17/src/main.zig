//! TODO: for part 2, at some point, I think the grid will repeat: this will
//! happen when the blocks create a 7 tile-wide floor, and the next block in
//! the sequence starts falling at the same position in the command cycle
//! that we were at when there was previously a 7 tile-wide floor!
//! We can also simply reset the grid when a 7 tile-wide floor is created, as the blocks
//! below will not affect the positions of any future blocks.
const std = @import("std");

const shapes = [_]u16{
    0b0000_0000_0000_1111,
    0b0000_0100_1110_0100,
    0b0000_0010_0010_1110,
    0b1000_1000_1000_1000,
    0b0000_0000_1100_1100,
};
const p1_max_height = 3 + 2022 * 4;
const cmds = blk: { // Just consider the first line.
    @setEvalBranchQuota(100_000);
    const input = @embedFile("input");
    break :blk input[0 .. std.mem.indexOf(u8, input, std.cstr.line_sep) orelse input.len];
};

fn shapeDo(grid: [][7]bool, shape_i: usize, shape_x: u8, shape_y: usize, comptime action: enum {
    validateX,
    validateY,
    rest,
}) union(enum) { is_valid: bool, min_y: usize } {
    var i: u8 = 0;
    var min_y: usize = std.math.maxInt(usize);
    var shape = shapes[shape_i % shapes.len];
    while (shape != 0) : ({
        i += 1;
        shape >>= 1;
    }) {
        if (shape & 1 == 0) continue;
        const x = shape_x + 3 - (i % 4);
        const y = shape_y + 3 - (i / 4);
        min_y = @min(min_y, y);
        switch (action) {
            .validateX => if (x >= 7 or grid[y][x]) break,
            .validateY => if (y >= grid.len or grid[y][x]) break,
            .rest => grid[y][x] = true,
        }
    }
    return switch (action) {
        .validateX, .validateY => .{ .is_valid = shape == 0 },
        .rest => .{ .min_y = min_y },
    };
}
pub fn main() !void {
    var grid = [_][7]bool{.{false} ** 7} ** p1_max_height;
    var min_y = grid.len;
    var shape_i: usize = 0;
    var shape_x: u8 = 2;
    var shape_y: usize = grid.len - 7;
    var cmd_i: usize = 0;
    while (shape_i < 2022) {
        const new_shape_x = if (cmds[cmd_i] == '<') shape_x -| 1 else shape_x + 1;
        cmd_i = (cmd_i + 1) % cmds.len;
        if (new_shape_x != shape_x and shapeDo(&grid, shape_i, new_shape_x, shape_y, .validateX).is_valid)
            shape_x = new_shape_x;
        if (!shapeDo(&grid, shape_i, shape_x, shape_y + 1, .validateY).is_valid) {
            min_y = @min(min_y, shapeDo(&grid, shape_i, shape_x, shape_y, .rest).min_y);
            shape_i += 1;
            shape_x = 2;
            shape_y = min_y - 7;
        } else shape_y += 1;
    }
    for (grid[min_y..]) |row| {
        std.debug.print("|", .{});
        for (row) |cell| std.debug.print("{s}", .{if (cell) "#" else "."});
        std.debug.print("|\n", .{});
    }
    std.debug.print("---------\n", .{});
    const stdout = std.io.getStdOut().writer();
    try stdout.print("Day17 (comptime parsing): P1: {}\n", .{p1_max_height - min_y});
}
