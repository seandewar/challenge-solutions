pub fn reverse(buffer: []u8, s: []const u8) []u8 {
    for (s, 0..) |c, i| buffer[s.len - 1 - i] = c;
    return buffer[0..s.len];
}
