// https://leetcode.com/problems/divide-two-integers
//
// Manual subtraction-based division.
//
// The 2s compliment negations can be written as x += x + x or x -= x + x
// depending on x's sign, if you want to avoid bitwise stuff, though XOR is
// still present for neg. The negation can also be written as -x, but that feels
// too much like multiplying by -1, so I didn't do it.
//
// The runtime can be improved to logarithmic if we instead divide using shifts.
// We can multiply via bitwise operations the divisor by the largest factor of
// 2^x that will fit, then decrease the factor to fill the remaining dividend
// until it's 0.
// e.g: 100 // 3 == 100 - 3*2^5 - 3*2^0 == 100 - 3*32 - 3*1 == 32 + 1 == 33
//
// See the Rust version of this Solution for an example of that approach.
//
// Complexity: runtime O(n), space O(1).

class Solution {
public:
    int divide(int dividend, int divisor) const noexcept
    {
        const bool neg = (dividend < 0) ^ (divisor < 0);

        // -ve holds a larger range than +ve, so by doing the division with -ve
        // ints, we avoid overflow if the inputs are INT_MIN
        if (dividend > 0)
            dividend = 1 + ~dividend;
        if (divisor > 0)
            divisor = 1 + ~divisor;

        int negResult = 0;
        if (divisor == -1)
            negResult = dividend; // identity
        else {
            for (; dividend <= divisor; --negResult)
                dividend -= divisor;
        }

        if (!neg && negResult == numeric_limits<int>::min())
            return numeric_limits<int>::max();

        return neg ? negResult : 1 + ~negResult;
    }
};
