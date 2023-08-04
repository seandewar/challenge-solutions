const std = @import("std");

pub const Classification = enum {
    deficient,
    perfect,
    abundant,
};

pub fn classify(n: u64) Classification {
    std.debug.assert(n != 0);

    var sum: u64 = 0;
    for (1..n / 2 + 1) |i| {
        const m: u64 = @intCast(i);
        if (n % m == 0) sum += m;
    }

    return switch (std.math.order(sum, n)) {
        .eq => .perfect,
        .gt => .abundant,
        .lt => .deficient,
    };
}
