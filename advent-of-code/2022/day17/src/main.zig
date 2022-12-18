//! TODO: for part 2, at some point, I think the grid will repeat: this will
//! happen when the blocks create a 7 tile-wide floor, and the next block in
//! the sequence starts falling at the same position in the command cycle
//! that we were at when there was previously a 7 tile-wide floor!
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
    var grid = [_][7]bool{.{false} ** 7} ** p1_max_height; // Index 0 is the bottom-most point of the grid.
    var floor_y: usize = 0;
    var max_y: usize = 0;
    var shape_i: usize = 0;
    var shape_x: u8 = 2; // Bottom-left of the shape.
    var shape_y: usize = 4;
    var cmd_i: usize = 0;
    var p1: usize = 0;
    while (true) {
        const new_shape_x = if (cmds[cmd_i] == '<') shape_x -| 1 else shape_x + 1;
        cmd_i = (cmd_i + 1) % cmds.len;
        if (new_shape_x != shape_x and shapeDo(&grid, floor_y, shape_i, new_shape_x, shape_y, .validateX).is_valid)
            shape_x = new_shape_x;
        if (!shapeDo(&grid, floor_y, shape_i, shape_x, shape_y - 1, .validateY).is_valid) {
            max_y = @max(max_y, shapeDo(&grid, floor_y, shape_i, shape_x, shape_y, .rest).max_y);
            // Check for any new 7-block floors formed by this piece. If so, we
            // can start the grid from after the new floor_y to save memory.
            var rev_it = std.mem.reverseIterator(grid[shape_y - 1 - floor_y .. shape_y - 1 - floor_y + 4]);
            while (rev_it.next()) |row| {
                if (!std.mem.allEqual(bool, &row, true)) continue;
                const new_floor_y = shape_y + rev_it.index;
                std.mem.copy([7]bool, &grid, grid[new_floor_y - floor_y .. max_y - floor_y]);
                std.mem.set([7]bool, grid[max_y - new_floor_y .. max_y - floor_y], .{false} ** 7);
                floor_y = new_floor_y;
                break;
            }
            shape_i += 1;
            shape_x = 2;
            shape_y = max_y + 4;
            if (shape_i == 2022) {
                p1 = max_y;
                break; // TODO: Part 2
            }
        } else shape_y -= 1;
    }
    const stdout = std.io.getStdOut().writer();
    try stdout.print("Day17 (comptime parsing): P1: {}, P2: {}\n", .{ p1, 0 });
}

fn debugPrintGrid(grid: []const [7]bool, floor_y: usize, max_y: usize) void {
    var rev_it = std.mem.reverseIterator(grid[0 .. max_y - floor_y]);
    while (rev_it.next()) |row| {
        std.debug.print("|", .{});
        for (row) |solid| std.debug.print("{s}", .{if (solid) "#" else "."});
        std.debug.print("|\n", .{});
    }
    std.debug.print("+-------+ <- y={}\n", .{floor_y});
}
