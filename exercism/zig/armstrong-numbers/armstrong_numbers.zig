pub fn isArmstrongNumber(num: u128) bool {
    var digits: u32 = 1;
    var x = num;
    while (x > 9) : (x /= 10) digits += 1;
    var sum: u128 = 0;
    x = num;
    while (x != 0) : (x /= 10)
        sum += @import("std").math.powi(u128, x % 10, digits) catch unreachable;
    return sum == num;
}
