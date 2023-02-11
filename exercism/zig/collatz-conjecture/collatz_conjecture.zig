pub const ComputationError = error{IllegalArgument};

pub fn steps(number: isize) ComputationError!usize {
    if (number <= 0) return error.IllegalArgument;
    if (number == 1) return 0;
    if (number & 1 == 0) return 1 + (steps(@divExact(number, 2)) catch unreachable);
    return 1 + (steps(number * 3 + 1) catch unreachable);
}
