pub const ChessboardError = error{IndexOutOfBounds};

pub fn square(index: isize) ChessboardError!u64 {
    if (index <= 0 or index > 64) return error.IndexOutOfBounds;
    return @as(u64, 1) << @intCast(u6, index - 1);
}

pub fn total() u64 {
    return @import("std").math.maxInt(u64); // All 64 bits will be set.
}
