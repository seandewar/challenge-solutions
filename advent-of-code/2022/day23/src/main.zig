const std = @import("std");
const parsed_elves = blk: {
    @setEvalBranchQuota(100_000);
    const input = @embedFile("input");
    var elf_count = 0;
    var line_it = std.mem.tokenize(u8, input, std.cstr.line_sep);
    while (line_it.next()) |line| elf_count += std.mem.count(u8, line, "#");
    var elves = std.BoundedArray(@Vector(2, i32), elf_count).init(0) catch unreachable;
    line_it.reset();
    var y = 0;
    while (line_it.next()) |line| : (y += 1)
        for (line) |c, x| if (c == '#') elves.append(.{ x, y }) catch unreachable;
    break :blk elves.buffer;
};

pub fn main() !void {
    const possible_moves = [_]struct { checks: [3]@Vector(2, i32), move: @Vector(2, i32) }{
        .{ .checks = .{ .{ -1, -1 }, .{ 0, -1 }, .{ 1, -1 } }, .move = .{ 0, -1 } }, // N
        .{ .checks = .{ .{ -1, 1 }, .{ 0, 1 }, .{ 1, 1 } }, .move = .{ 0, 1 } }, // S
        .{ .checks = .{ .{ -1, -1 }, .{ -1, 0 }, .{ -1, 1 } }, .move = .{ -1, 0 } }, // W
        .{ .checks = .{ .{ 1, -1 }, .{ 1, 0 }, .{ 1, 1 } }, .move = .{ 1, 0 } }, // E
    };
    const Move = struct { elf_i: usize, to: @Vector(2, i32) };
    var elves = parsed_elves;
    var moves = std.BoundedArray(Move, elves.len).init(0) catch unreachable;
    const stdout = std.io.getStdOut().writer();
    var round: u32 = 0;
    while (round == 0 or moves.len > 0) : (round += 1) {
        if (round == 10) { // Assumes computing part 2 takes more than 10 rounds.
            var rect_bounds = [_]@Vector(2, i32){elves[0]} ** 2;
            for (elves[1..]) |elf_pos| {
                rect_bounds[0] = @min(rect_bounds[0], elf_pos);
                rect_bounds[1] = @max(rect_bounds[1], elf_pos);
            }
            const p1 = @intCast(usize, @reduce(.Mul, rect_bounds[1] + @splat(2, @as(i32, 1)) - rect_bounds[0])) - elves.len;
            try stdout.print("Day23 (comptime parsing): P1: {}", .{p1});
        }
        moves.len = 0;
        std.sort.sort(@Vector(2, i32), &elves, {}, struct {
            fn lessThan(context: void, lhs: @Vector(2, i32), rhs: @Vector(2, i32)) bool {
                _ = context;
                return lhs[0] < rhs[0] or (lhs[0] == rhs[0] and lhs[1] < rhs[1]);
            }
        }.lessThan);
        for (elves) |elf_pos, elf_i| {
            var has_adj_elf = false;
            var next_move: ?Move = null;
            next_move_dir: for (possible_moves) |_, i| {
                const info = possible_moves[(round + i) % possible_moves.len];
                for (info.checks) |check_dir| {
                    if (std.sort.binarySearch(@Vector(2, i32), elf_pos + check_dir, &elves, {}, struct {
                        fn compareFn(context: void, lhs: @Vector(2, i32), rhs: @Vector(2, i32)) std.math.Order {
                            _ = context;
                            if (lhs[0] < rhs[0] or (lhs[0] == rhs[0] and lhs[1] < rhs[1])) {
                                return .lt;
                            } else if (@reduce(.And, lhs == rhs)) {
                                return .eq;
                            } else return .gt;
                        }
                    }.compareFn) != null) {
                        has_adj_elf = true;
                        continue :next_move_dir;
                    }
                }
                if (next_move == null) next_move = .{ .elf_i = elf_i, .to = elf_pos + info.move };
            }
            if (has_adj_elf and next_move != null) moves.append(next_move.?) catch unreachable;
        }
        std.sort.sort(Move, moves.slice(), {}, struct {
            fn lessThan(context: void, lhs: Move, rhs: Move) bool {
                _ = context;
                return lhs.to[0] < rhs.to[0] or (lhs.to[0] == rhs.to[0] and lhs.to[1] < rhs.to[1]);
            }
        }.lessThan);
        for (moves.slice()) |move, i| {
            if ((i > 0 and @reduce(.And, move.to == moves.get(i - 1).to)) or
                (i + 1 < moves.len and @reduce(.And, move.to == moves.get(i + 1).to))) continue;
            elves[move.elf_i] = move.to;
        }
    }
    try stdout.print(", P2: {}\n", .{round});
}
