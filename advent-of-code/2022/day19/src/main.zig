const std = @import("std");
const Resources = struct { ore: u32 = 0, clay: u32 = 0, obsidian: u32 = 0, geode: u32 = 0 };
const Blueprint = struct {
    bot_ore_costs: Resources,
    max_bot_ore_cost: u32,
    obsidian_bot_clay_cost: u32,
    geode_bot_obsidian_cost: u32,
};
const parsed_blueprints = blk: {
    @setEvalBranchQuota(100_000);
    const input = @embedFile("input");
    var blueprint_count = 0;
    var line_it = std.mem.tokenize(u8, input, std.cstr.line_sep);
    while (line_it.next()) |_| blueprint_count += 1;
    var blueprints: [blueprint_count]Blueprint = undefined;
    line_it.reset();
    for (blueprints) |*blueprint| {
        var cost_it = std.mem.split(u8, line_it.next().?, "costs ");
        _ = cost_it.next().?;
        var strs: [4][]const u8 = undefined;
        for (strs) |*str| str.* = cost_it.next().?;
        const obsidian_after_i = std.mem.indexOfScalar(u8, strs[2], ' ').?;
        const geode_after_i = std.mem.indexOfScalar(u8, strs[3], ' ').?;
        blueprint.* = .{
            .bot_ore_costs = .{
                .ore = std.fmt.parseInt(u32, strs[0][0..std.mem.indexOfScalar(u8, strs[0], ' ').?], 10) catch unreachable,
                .clay = std.fmt.parseInt(u32, strs[1][0..std.mem.indexOfScalar(u8, strs[1], ' ').?], 10) catch unreachable,
                .obsidian = std.fmt.parseInt(u32, strs[2][0..obsidian_after_i], 10) catch unreachable,
                .geode = std.fmt.parseInt(u32, strs[3][0..geode_after_i], 10) catch unreachable,
            },
            .max_bot_ore_cost = 0,
            .obsidian_bot_clay_cost = std.fmt.parseInt(u32, strs[2][obsidian_after_i + " ore and ".len .. strs[2].len - " clay. Each geode robot ".len], 10) catch unreachable,
            .geode_bot_obsidian_cost = std.fmt.parseInt(u32, strs[3][geode_after_i + " ore and ".len .. strs[3].len - " obsidian.".len], 10) catch unreachable,
        };
        inline for (@typeInfo(Resources).Struct.fields) |field|
            blueprint.max_bot_ore_cost = @max(blueprint.max_bot_ore_cost, @field(blueprint.bot_ore_costs, field.name));
    }
    break :blk blueprints;
};

fn computeMaxGeodes(blueprint: Blueprint, rem_time: u32, resources: *Resources, bots: *Resources) u32 {
    const prev_resources = resources.*;
    var maxGeodes = resources.geode + rem_time * bots.geode;
    inline for ([_]struct { bot: []const u8, extra_cost: ?[]const u8 = null }{
        .{ .bot = "ore" },
        .{ .bot = "clay" },
        .{ .bot = "obsidian", .extra_cost = "clay" },
        .{ .bot = "geode", .extra_cost = "obsidian" },
    }) |info, info_i| {
        const extra_resource_prod: ?u32 = if (info.extra_cost) |cost| @field(bots, cost) else null;
        const max_bot_count: u32 = switch (info_i) { // No need to make a bot if we're producing more resources than we can spend.
            0 => blueprint.max_bot_ore_cost,
            1 => blueprint.obsidian_bot_clay_cost,
            2 => blueprint.geode_bot_obsidian_cost,
            else => std.math.maxInt(u32),
        };
        const bot_count = &@field(bots, info.bot);
        if ((extra_resource_prod == null or extra_resource_prod.? > 0) and bot_count.* < max_bot_count) { // inline for can't continue via runtime control flow. :(
            const ore_cost = @field(blueprint.bot_ore_costs, info.bot);
            const extra_resource: ?struct { count: *u32, cost: u32 } = if (info.extra_cost) |cost| .{
                .count = &@field(resources, cost),
                .cost = @field(blueprint, info.bot ++ "_bot_" ++ cost ++ "_cost"),
            } else null;
            const build_time = 1 + @max(
                std.math.divCeil(u32, ore_cost -| resources.ore, bots.ore) catch unreachable,
                if (extra_resource) |resource| (std.math.divCeil(u32, resource.cost -| resource.count.*, extra_resource_prod.?) catch unreachable) else 0,
            );
            if (rem_time > build_time) {
                inline for (@typeInfo(Resources).Struct.fields) |field| @field(resources, field.name) += build_time * @field(bots, field.name);
                resources.ore -= ore_cost;
                if (extra_resource) |resource| resource.count.* -= resource.cost;
                bot_count.* += 1;
                maxGeodes = @max(maxGeodes, computeMaxGeodes(blueprint, rem_time - build_time, resources, bots));
                bot_count.* -= 1; // Start back-tracking.
                resources.* = prev_resources;
            }
        }
    }
    return maxGeodes;
}
fn computeAnswer(comptime p1: bool) usize {
    var result: usize = if (p1) 0 else 1;
    for (if (p1) parsed_blueprints else parsed_blueprints[0..3]) |blueprint, blueprint_i| {
        var resources = Resources{ .ore = @min(blueprint.bot_ore_costs.ore, blueprint.bot_ore_costs.clay) };
        var bots = Resources{ .ore = 1 };
        if (p1)
            result += (blueprint_i + 1) * computeMaxGeodes(blueprint, 24 -| resources.ore, &resources, &bots)
        else
            result *= computeMaxGeodes(blueprint, 32 -| resources.ore, &resources, &bots);
    }
    return result;
}
pub fn main() !void {
    const stdout = std.io.getStdOut().writer();
    try stdout.print("Day19 (comptime parsing): P1: {}", .{computeAnswer(true)});
    try stdout.print(", P2: {}\n", .{computeAnswer(false)});
}
