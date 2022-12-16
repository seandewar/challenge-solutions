const std = @import("std");

const parsed_input = blk: {
    @setEvalBranchQuota(100_000);
    const input = @embedFile("input");
    var sensor_count = 0;
    var line_it = std.mem.tokenize(u8, input, std.cstr.line_sep);
    while (line_it.next()) |_| sensor_count += 1;
    var sensors: [sensor_count]struct { sx: i32, sy: i32, bx: i32, by: i32 } = undefined;
    var p1_bxs_on_y = std.BoundedArray(i32, sensor_count).init(0) catch unreachable;
    line_it = std.mem.tokenize(u8, input, std.cstr.line_sep);
    for (sensors) |*sensor| {
        const line = line_it.next().?;
        var start_i: usize = 0;
        for (.{ "sx", "sy", "bx", "by" }) |field| {
            const i = std.mem.indexOfScalarPos(u8, line, start_i, '=').? + 1;
            start_i = std.mem.indexOfAnyPos(u8, line, i, ",:") orelse line.len;
            @field(sensor.*, field) = std.fmt.parseInt(i32, line[i..start_i], 10) catch unreachable;
        }
        if (sensor.by == 2_000_000) p1_bxs_on_y.append(sensor.bx) catch unreachable;
    }
    if (p1_bxs_on_y.len != 0) { // Sort and dedup bxs on line y=2,000,000.
        std.sort.sort(i32, p1_bxs_on_y.slice(), {}, std.sort.asc(i32));
        const slice = p1_bxs_on_y.slice();
        p1_bxs_on_y.len = 1;
        for (slice[1..]) |bx, prev_i| if (bx != slice[prev_i]) p1_bxs_on_y.append(bx) catch unreachable;
    }
    break :blk .{ .sensors = sensors, .p1_bxs_on_y = p1_bxs_on_y.slice() };
};

const Interval = struct { x0: i32, x1: i32 };
fn computeExclusions(y: i32, p2: bool) std.BoundedArray(Interval, parsed_input.sensors.len) {
    var intervals = std.BoundedArray(Interval, parsed_input.sensors.len).init(0) catch unreachable;
    for (parsed_input.sensors) |sensor| {
        const d = std.math.absCast(sensor.by - sensor.sy) + std.math.absCast(sensor.bx - sensor.sx); // Manhattan dist.
        if (std.math.absCast(sensor.sy - y) > d) continue; // Line further away than the beacon.
        // On the y line, all x where d>=|sy-y|+|sx-x| can't have other beacons.
        // To find the exclusion interval, solve for the two values of x where the rhs equals d.
        const xa = sensor.sx + (std.math.absInt(sensor.sy - y) catch unreachable) - @intCast(i32, d);
        const xb = sensor.sx - (std.math.absInt(sensor.sy - y) catch unreachable) + @intCast(i32, d);
        var interval = Interval{ .x0 = @min(xa, xb), .x1 = @max(xa, xb) };
        if (p2) {
            if (interval.x1 < 0 or interval.x0 > 4_000_000) continue;
            interval = .{ .x0 = @max(0, interval.x0), .x1 = @min(4_000_000, interval.x1) };
        }
        intervals.append(interval) catch unreachable;
    }
    if (intervals.len < 2) return intervals;
    std.sort.sort(Interval, intervals.slice(), {}, struct { // Sort so interval merging is easy.
        fn lessThan(context: void, lhs: Interval, rhs: Interval) bool {
            _ = context;
            return lhs.x0 < rhs.x0; // Sorting x1 doesn't matter.
        }
    }.lessThan);
    const slice = intervals.slice();
    var interval_accum = intervals.get(0);
    intervals.len = 0;
    for (slice[1..]) |interval| { // Merge overlapping intervals in-place.
        if (interval_accum.x1 < interval.x0) { // Can't merge with previous.
            intervals.append(interval_accum) catch unreachable;
            interval_accum = interval;
        } else interval_accum.x1 = @max(interval_accum.x1, interval.x1);
    }
    intervals.append(interval_accum) catch unreachable;
    return intervals;
}
const p1 = blk: {
    @setEvalBranchQuota(100_000);
    var result = 0;
    for (computeExclusions(2_000_000, false).slice()) |interval| {
        result += 1 + interval.x1 - interval.x0;
        for (parsed_input.p1_bxs_on_y) |bx| { // Compensate for known beacons on the line.
            if (bx >= interval.x0 and bx <= interval.x1) result -= 1;
        }
    }
    break :blk result;
};

pub fn main() !void {
    const stdout = std.io.getStdOut().writer();
    try stdout.print("Day15 (P1 at comptime): P1: {}", .{p1});
    var p2_y: i32 = 0;
    while (p2_y <= 4_000_000) : (p2_y += 1) {
        const interval = computeExclusions(p2_y, true).get(0);
        if (interval.x0 == 0 and interval.x1 == 4_000_000) continue;
        const p2_x: i64 = if (interval.x0 != 0) interval.x0 - 1 else interval.x1 + 1;
        try stdout.print(", P2: {}\n", .{4_000_000 * p2_x + p2_y});
        return;
    }
}
