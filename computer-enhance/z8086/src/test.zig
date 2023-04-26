const std = @import("std");

test {
    _ = @import("decode.zig");
    _ = @import("main.zig");
}

pub fn testOpenListing(name: []const u8) !std.fs.File {
    const listings_dir = std.fs.cwd().openDir("../computer_enhance/perfaware/part1", .{}) catch |err| switch (err) {
        error.FileNotFound => {
            std.log.err("Could not find the \"Computer, Enhance!\" listings directory! " ++
                "Make sure you clone submodules and run tests from the z8086 directory.", .{});
            return error.SkipZigTest;
        },
        else => return err,
    };

    var buf: [std.fs.MAX_PATH_BYTES - 1:0]u8 = undefined;
    return try listings_dir.openFileZ(try std.fmt.bufPrintZ(&buf, "listing_{s}", .{name}), .{});
}
