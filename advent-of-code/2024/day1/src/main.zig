const std = @import("std");

pub fn main() !void {
    var arena_instance: std.heap.ArenaAllocator = .init(std.heap.page_allocator);
    defer arena_instance.deinit();
    const arena = arena_instance.allocator();

    const input = try std.fs.cwd().readFileAlloc(arena, "src/input", std.math.maxInt(u32));
    var number_it = std.mem.tokenizeAny(u8, input, " \r\n");
    var capacity: usize = 0;
    while (number_it.next()) |_| : (capacity += 1)
        if (number_it.next() == null) return error.MissingNumber;

    var left_nums: std.ArrayListUnmanaged(u32) = try .initCapacity(arena, capacity);
    var right_nums: std.ArrayListUnmanaged(u32) = try .initCapacity(arena, capacity);
    var right_freqs: std.AutoHashMapUnmanaged(u32, u32) = .empty;
    number_it.reset();
    while (number_it.next()) |l_str| {
        const l = try std.fmt.parseInt(u32, l_str, 10);
        const r = try std.fmt.parseInt(u32, number_it.next().?, 10);
        left_nums.appendAssumeCapacity(l);
        right_nums.appendAssumeCapacity(r);
        (try right_freqs.getOrPutValue(arena, r, 0)).value_ptr.* += 1;
    }
    std.mem.sortUnstable(u32, left_nums.items, {}, std.sort.asc(u32));
    std.mem.sortUnstable(u32, right_nums.items, {}, std.sort.asc(u32));

    var p1: u32 = 0;
    var p2: u32 = 0;
    for (left_nums.items, right_nums.items) |l, r| {
        p1 += @max(l, r) - @min(l, r);
        p2 += l * (right_freqs.get(l) orelse 0);
    }
    try std.io.getStdOut().writer().print("Day1: P1: {}, P2: {}\n", .{ p1, p2 });
}
