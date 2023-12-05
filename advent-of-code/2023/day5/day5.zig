const std = @import("std");

const Interval = struct { start: u64, len: u64 };
const IntervalList = std.ArrayList(Interval);

fn sortAndMergeIntervals(intervals: *IntervalList) void {
    const items = intervals.items;
    std.mem.sortUnstable(Interval, items, {}, opaque {
        fn lessThan(_: void, l: Interval, r: Interval) bool {
            return l.start < r.start or (l.start == r.start and l.len >= r.len);
        }
    }.lessThan);

    var merge_iv = if (items.len > 0) items[0] else return;
    var i: usize = 1;
    var j: usize = 0;
    while (i < items.len) : (i += 1) { // Could bisect for the next largest start as larger lens come first, but we good.
        if (merge_iv.start + merge_iv.len > items[i].start) {
            merge_iv.len = @max(merge_iv.len, items[i].len + items[i].start - merge_iv.start);
            continue;
        }
        if (merge_iv.len > 0) {
            items[j] = merge_iv;
            j += 1;
        }
        merge_iv = items[i];
    }
    if (merge_iv.len > 0) {
        items[j] = merge_iv;
        j += 1;
    }
    intervals.resize(j) catch unreachable;
}

fn translateIntervals(ivs: *IntervalList, new_ivs: *IntervalList, dst: u64, src: u64, len: u64) !void {
    var i: usize = 0;
    while (i < ivs.items.len) : (i += 1) { // May realloc `items` below, which is why we while loop.
        const iv = ivs.items[i];
        if (src + len <= iv.start or src >= iv.start + iv.len) continue;
        ivs.items[i].len = if (src >= iv.start) src - iv.start else 0;
        if (iv.start + iv.len > src + len) try ivs.append(.{ .start = src + len, .len = iv.start + iv.len - src - len });

        try new_ivs.append(.{ .start = dst + @max(iv.start, src) - src, .len = @min(iv.len, len) });
    }
}

pub fn main() !void {
    const file = try std.fs.cwd().openFile("input", .{});
    defer file.close();
    var buf_reader = std.io.bufferedReader(file.reader());
    const reader = buf_reader.reader();

    var arena = std.heap.ArenaAllocator.init(std.heap.page_allocator);
    defer arena.deinit();

    var line_buf = try std.ArrayList(u8).initCapacity(arena.allocator(), 512);
    try reader.streamUntilDelimiter(line_buf.writer(), '\n', null);
    var tok_it = std.mem.tokenizeScalar(u8, line_buf.items, ' ');
    if (!std.mem.eql(u8, tok_it.next() orelse return error.BadInput, "seeds:")) return error.BadInput;

    var p1_seeds = try IntervalList.initCapacity(arena.allocator(), 32);
    var p1_new_seeds = try IntervalList.initCapacity(arena.allocator(), 64);
    var p2_seeds = try IntervalList.initCapacity(arena.allocator(), 128);
    var p2_new_seeds = try IntervalList.initCapacity(arena.allocator(), 256);

    while (tok_it.next()) |tok| {
        const a = try std.fmt.parseUnsigned(u64, tok, 10);
        const b = try std.fmt.parseUnsigned(u64, tok_it.next() orelse return error.BadInput, 10);

        try p1_seeds.appendSlice(&.{ .{ .start = a, .len = 1 }, .{ .start = b, .len = 1 } });
        try p2_seeds.append(.{ .start = a, .len = b });
    }
    if (p1_seeds.items.len == 0) return error.BadInput;
    sortAndMergeIntervals(&p1_seeds);
    sortAndMergeIntervals(&p2_seeds);

    while (true) {
        line_buf.clearRetainingCapacity();
        reader.streamUntilDelimiter(line_buf.writer(), '\n', null) catch |err| switch (err) {
            error.EndOfStream => break,
            else => return err,
        };

        tok_it = std.mem.tokenizeScalar(u8, line_buf.items, ' ');
        if (tok_it.next() == null) continue;
        if (!std.mem.eql(u8, tok_it.next() orelse return error.BadInput, "map:")) return error.BadInput;
        if (tok_it.next() != null) return error.BadInput;

        while (true) {
            line_buf.clearRetainingCapacity();
            reader.streamUntilDelimiter(line_buf.writer(), '\n', null) catch |err| switch (err) {
                error.EndOfStream => break,
                else => return err,
            };

            tok_it = std.mem.tokenizeScalar(u8, line_buf.items, ' ');
            const dst = try std.fmt.parseUnsigned(u64, tok_it.next() orelse break, 10);
            const src = try std.fmt.parseUnsigned(u64, tok_it.next() orelse return error.BadInput, 10);
            const len = try std.fmt.parseUnsigned(u64, tok_it.next() orelse return error.BadInput, 10);
            if (tok_it.next() != null) return error.BadInput;

            try translateIntervals(&p1_seeds, &p1_new_seeds, dst, src, len);
            try translateIntervals(&p2_seeds, &p2_new_seeds, dst, src, len);
        }

        try p1_seeds.appendSlice(p1_new_seeds.items);
        try p2_seeds.appendSlice(p2_new_seeds.items);
        p1_new_seeds.clearRetainingCapacity();
        p2_new_seeds.clearRetainingCapacity();
        sortAndMergeIntervals(&p1_seeds);
        sortAndMergeIntervals(&p2_seeds);
    }

    const stdout = std.io.getStdOut().writer();
    try stdout.print("Day5: P1: {}, P2: {}\n", .{ p1_seeds.items[0].start, p2_seeds.items[0].start });
}
