//! This solution is unlikely to work for *all* possible inputs, but should
//! work for most. To increase the accuracy of this solution, change `seen_map`
//! to remember more rows to compare with later.
const std = @import("std");
const shapes = [_]u16{
    0b0000_0000_0000_1111,
    0b0000_0100_1110_0100,
    0b0000_0010_0010_1110,
    0b1000_1000_1000_1000,
    0b0000_0000_1100_1100,
};
const shape_heights = [_]usize{ 1, 3, 3, 4, 2 };
const cmds = blk: { // Just consider the first line.
    @setEvalBranchQuota(100_000);
    const input = @embedFile("input");
    break :blk input[0 .. std.mem.indexOf(u8, input, std.cstr.line_sep) orelse input.len];
};

fn shapeDo(grid: [][7]bool, max_ys: *[7]usize, shape_i: usize, shape_x: u8, shape_y: usize, comptime action: enum {
    validateX,
    validateY,
    rest,
}) !bool {
    const max_y = std.mem.max(usize, max_ys);
    var i: u8 = 0;
    var shape = shapes[shape_i];
    while (shape != 0) : ({
        i += 1;
        shape >>= 1;
    }) {
        if (shape & 1 == 0) continue;
        const x = shape_x + 3 - (i % 4);
        const y = shape_y + i / 4;
        switch (action) {
            .validateX => if (x >= 7 or (y <= max_y and grid[(y - 1) % grid.len][x])) return false,
            .validateY => if (y <= max_y -| grid.len or (y <= max_y and grid[(y - 1) % grid.len][x]))
                return if (y > 0 and y <= max_y -| grid.len) error.GridTooSmall else false,
            .rest => {
                max_ys.*[x] = @max(max_ys.*[x], y);
                grid[(y - 1) % grid.len][x] = true;
            },
        }
    }
    return true;
}
pub fn main() !void {
    const stdout = std.io.getStdOut().writer();
    var arena = std.heap.ArenaAllocator.init(std.heap.page_allocator);
    defer arena.deinit();
    const allocator = arena.allocator();
    var seen_map = std.AutoHashMap(struct {
        cmd_i: usize,
        shape_i: usize,
        y_diffs: [7]usize,
    }, struct { shape_num: usize, max_y: usize }).init(allocator);
    var grid = [_][7]bool{.{false} ** 7} ** 512;
    var max_ys = [_]usize{0} ** 7;
    var shape_num: usize = 1;
    var max_y_skip: usize = 0;
    var shape_num_skip: usize = 0;
    var shape_x: u8 = 2; // Bottom-left of the shape.
    var shape_y: usize = 4;
    var cmd_i: usize = 0;
    while (shape_num + shape_num_skip <= 1_000_000_000_000) {
        const shape_i = (shape_num - 1) % shapes.len;
        const new_shape_x = if (cmds[cmd_i] == '<') shape_x -| 1 else shape_x + 1;
        cmd_i = (cmd_i + 1) % cmds.len;
        if (new_shape_x != shape_x and try shapeDo(&grid, &max_ys, shape_i, new_shape_x, shape_y, .validateX))
            shape_x = new_shape_x;
        if (!try shapeDo(&grid, &max_ys, shape_i, shape_x, shape_y - 1, .validateY)) {
            const shape_top_y = shape_y + shape_heights[shape_i] - 1;
            var max_y = std.mem.max(usize, &max_ys);
            while (max_y < shape_top_y) : (max_y += 1) // Scroll the grid if necessary.
                if (max_y >= grid.len) std.mem.set(bool, &grid[(max_y - grid.len) % grid.len], false);
            _ = try shapeDo(&grid, &max_ys, shape_i, shape_x, shape_y, .rest);
            if (shape_num == 2022) try stdout.print("Day17 (comptime parsing): P1: {}", .{max_y});
            var y_diffs: [7]usize = undefined;
            for (y_diffs) |*diff, x| diff.* = max_y - max_ys[x];
            const seen = try seen_map.getOrPut(.{ .cmd_i = cmd_i, .shape_i = shape_i, .y_diffs = y_diffs });
            if (seen.found_existing and shape_num >= 2022) { // Skip ahead for P2.
                const shape_count = shape_num - seen.value_ptr.shape_num;
                const mul = (1_000_000_000_000 - shape_num - shape_num_skip) / shape_count;
                shape_num_skip += shape_count * mul;
                max_y_skip += (max_y - seen.value_ptr.max_y) * mul;
            } else if (!seen.found_existing) seen.value_ptr.* = .{ .shape_num = shape_num, .max_y = max_y };
            if (shape_num + shape_num_skip == 1_000_000_000_000) try stdout.print(", P2: {}\n", .{max_y + max_y_skip});
            shape_num += 1;
            shape_x = 2;
            shape_y = max_y + 4;
        } else shape_y -= 1;
    }
}
