const std = @import("std");
const parsed_input = blk: {
    @setEvalBranchQuota(100_000);
    const input = @embedFile("input");
    var height = 0;
    var line_it = std.mem.tokenize(u8, input, std.cstr.line_sep);
    while (line_it.next()) |_| height += 1;
    line_it.reset();
    var grid: [height][]const u8 = undefined;
    for (grid) |*row| row.* = line_it.next().?;
    break :blk .{ .grid = grid, .width = grid[0].len, .height = height };
};

fn computeAnswer(start_time: u32, comptime from_end: bool, stacks: *[2]std.ArrayList(@Vector(2, i32))) !u32 {
    var time: u32 = start_time;
    stacks[time % 2].clearRetainingCapacity();
    try stacks[time % 2].append(if (from_end) .{ parsed_input.width - 2, parsed_input.height - 1 } else .{ 1, 0 });
    while (true) : (time += 1) {
        const back_stack = &stacks[(time + 1) % 2];
        back_stack.clearRetainingCapacity();
        for (stacks[time % 2].items) |pos| {
            for ([_]@Vector(2, i32){ .{ 1, 0 }, .{ -1, 0 }, .{ 0, 1 }, .{ 0, -1 }, .{ 0, 0 } }) |dir| {
                const next_pos = pos + dir;
                if (next_pos[1] < 0 or next_pos[1] >= parsed_input.height or next_pos[0] < 0 or next_pos[0] >= parsed_input.width) continue;
                const i = @Vector(2, usize){ @intCast(usize, next_pos[0]), @intCast(usize, next_pos[1]) };
                if (parsed_input.grid[i[1]][i[0]] == '#') continue;
                if ((from_end and i[1] == 0) or (!from_end and i[1] == parsed_input.height - 1)) return time;
                if ((i[1] != 0 and i[1] != parsed_input.height - 1) and // No storms reach the start or end rows.
                    (parsed_input.grid[i[1]][1 + @intCast(usize, @mod(next_pos[0] - @intCast(i32, time) - 1, @intCast(i32, parsed_input.width - 2)))] == '>' or
                    parsed_input.grid[i[1]][1 + @mod(i[0] + time - 1, parsed_input.width - 2)] == '<' or
                    parsed_input.grid[1 + @intCast(usize, @mod(next_pos[1] - @intCast(i32, time) - 1, @intCast(i32, parsed_input.height - 2)))][i[0]] == 'v' or
                    parsed_input.grid[1 + @mod(i[1] + time - 1, parsed_input.height - 2)][i[0]] == '^'))
                    continue;
                try back_stack.append(next_pos);
            }
        }
        std.sort.sort(@Vector(2, i32), back_stack.items, {}, struct { // Sort to dedup.
            fn lessThan(context: void, lhs: @Vector(2, i32), rhs: @Vector(2, i32)) bool {
                _ = context;
                return lhs[0] < rhs[0] or (lhs[0] == rhs[0] and lhs[1] < rhs[1]);
            }
        }.lessThan);
        var new_len: usize = 1;
        for (back_stack.items[1..]) |pos, prev_i| {
            if (@reduce(.And, pos == back_stack.items[prev_i])) continue;
            if (prev_i + 1 != new_len) back_stack.items[new_len] = pos;
            new_len += 1;
        }
        back_stack.shrinkRetainingCapacity(new_len);
    }
}
pub fn main() !void {
    var arena = std.heap.ArenaAllocator.init(std.heap.page_allocator);
    defer arena.deinit();
    const alloc = arena.allocator();
    var stacks = [_]std.ArrayList(@Vector(2, i32)){std.ArrayList(@Vector(2, i32)).init(alloc)} ** 2; // Use 2 stacks as I cba to use a queue.
    defer for (stacks) |*stack| stack.deinit();
    const p1 = try computeAnswer(0, false, &stacks);
    const stdout = std.io.getStdOut().writer();
    try stdout.print("Day24 (comptime parsing): P1: {}", .{p1});
    try stdout.print(", P2: {}\n", .{try computeAnswer(try computeAnswer(p1, true, &stacks), false, &stacks)});
}
