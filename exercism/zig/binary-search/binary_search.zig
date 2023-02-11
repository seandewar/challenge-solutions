pub const SearchError = error{ ValueAbsent, EmptyBuffer };

pub fn binarySearch(comptime T: type, target: T, buffer: []const T) SearchError!usize {
    if (buffer.len == 0) return error.EmptyBuffer;
    var l: usize = 0;
    var r = buffer.len;
    while (l < r) {
        const m = l + (r - 1 - l) / 2;
        if (buffer[m] == target) return m;
        if (buffer[m] < target) l = m + 1 else r = m;
    }
    return error.ValueAbsent;
}
