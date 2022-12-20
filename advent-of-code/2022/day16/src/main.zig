const std = @import("std");

const parsed_input = blk: {
    @setEvalBranchQuota(1_000_000);
    const input = @embedFile("input");
    var valve_count = 0;
    var line_it = std.mem.tokenize(u8, input, std.cstr.line_sep);
    while (line_it.next()) |_| valve_count += 1;
    var valves: [valve_count]struct { name: []const u8, flow_rate: u8, to_is: []usize } = undefined;
    var start_i: u8 = undefined;
    line_it.reset();
    for (valves) |*valve, valve_i| {
        const line = line_it.next().?;
        const flow_rate_str = line[23..std.mem.indexOfScalarPos(u8, line, 24, ';').?];
        valve.* = .{
            .name = line[6..8],
            .flow_rate = std.fmt.parseInt(u8, flow_rate_str, 10) catch unreachable,
            .to_is = undefined,
        };
        if (std.mem.eql(u8, valve.name, "AA")) start_i = valve_i;
    }
    line_it.reset();
    for (valves) |*valve| {
        var to_is = std.BoundedArray(usize, valve_count).init(0) catch unreachable;
        var to_it = std.mem.split(u8, line_it.next().?, ", ");
        while (to_it.next()) |to_str| {
            for (valves) |to_valve, to_i| {
                if (std.mem.eql(u8, to_valve.name, to_str[to_str.len - 2 ..])) {
                    to_is.append(to_i) catch unreachable;
                    break;
                }
            }
        }
        valve.to_is = to_is.slice();
    }
    var min_dists = [_][valve_count]u8{.{0xff} ** valve_count} ** valve_count;
    for (min_dists) |*dists, valve_i| {
        dists.*[valve_i] = 0;
        var step = 0;
        var relax_next_step = step;
        while (relax_next_step == step) : (step += 1) {
            for (dists.*) |dist, from_i| {
                if (dist != step) continue;
                for (valves[from_i].to_is) |to_i| {
                    if (dists.*[to_i] != 0xff) continue;
                    dists.*[to_i] = dist + 1;
                    relax_next_step = step + 1;
                }
            }
        }
    }
    break :blk .{ .valves = valves, .min_dists = min_dists, .start_i = start_i };
};

fn computeFlow(open_valves: *[parsed_input.valves.len]bool, time: u8, helpers: [2]struct { valve_i: usize = parsed_input.start_i, time: u8 = 0 }) u16 {
    for (helpers) |helper, helper_i| {
        if (helper.time != 0) continue;
        var max_flow: u16 = 0;
        for (parsed_input.min_dists[helper.valve_i]) |dist, to_i| {
            if (open_valves.*[to_i] or time <= dist + 1 or parsed_input.valves[to_i].flow_rate == 0) continue;
            open_valves.*[to_i] = true;
            var new_helpers = helpers;
            new_helpers[helper_i] = .{ .valve_i = to_i, .time = dist + 1 };
            max_flow = @max(max_flow, computeFlow(open_valves, time, new_helpers));
            open_valves.*[to_i] = false; // Backtrack.
        }
        if (max_flow == 0) { // Helper has no more possible moves.
            var new_helpers = helpers;
            new_helpers[helper_i] = .{ .time = 0xff };
            max_flow = computeFlow(open_valves, time, new_helpers); // Other helper may not be finished yet.
        }
        return max_flow + @as(u16, parsed_input.valves[helper.valve_i].flow_rate) * time;
    }
    const time_advance = @min(helpers[0].time, helpers[1].time);
    if (time <= time_advance) return 0;
    var new_helpers = helpers;
    for (new_helpers) |*helper| helper.time -= time_advance;
    return computeFlow(open_valves, time - time_advance, new_helpers);
}
pub fn main() !void {
    const stdout = std.io.getStdOut().writer();
    var open_valves = [_]bool{false} ** parsed_input.valves.len;
    try stdout.print("Day16 (comptime parsing): P1: {}", .{computeFlow(&open_valves, 30, .{ .{}, .{ .time = 0xff } })});
    try stdout.print(", P2: {}\n", .{computeFlow(&open_valves, 26, .{ .{}, .{} })});
}
