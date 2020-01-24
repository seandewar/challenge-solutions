// https://leetcode.com/problems/power-of-three/
//
// Highest power of 3 <= Integer.MAX_VALUE modulo method [3^19 for Java
// ints, which are 32-bits wide & signed]. This number can be calculated as:
//
// (int)(log(Integer.MAX_VALUE) / log(3)) == 19 for 32-bit signed integers.
//
// This particular method only works if the base is a prime number (which 3
// is), which allows 3^19 to only be evenly divisible by 3 along with all other
// powers of 3 from 3^0 to 3^19. As 3^20 is outside the range of a signed
// 32-bit int, 3^19 covers all possible powers of 3 within Java's int range.
//
// This is the fastest of all the methods listed here, but probably the least
// immediately digestible one to begin with (but rather simple to understand
// once you get over the initially jarring magic number).
//
// Complexity: runtime O(1), space O(1).

class Solution {
    public boolean isPowerOfThree(final int n) {
        return n > 0 && 1162261467 % n == 0;
    }
}

// Alternative Solution: log() method (natural log [base e log], ln()).
//
// Given that 3^x = n, ln(3^x) = ln(n), x*ln(3) = ln(n), x = ln(n)/ln(3).
//
// We then test if x is an integer by calculating the difference between x and
// its nearest integer floating point value, then we compare it against some
// epsilon value to account for FP rounding errors (10^-10 works) [abs() is
// used as x might be slightly smaller than the actual power value due to FP
// errors].
//
// On a side note, this problem can of course be solved iteratively and
// recursively too (albeit at a greater runtime/space complexity). Just test if
// n is evenly divisible by 3, divide by 3 if it is and repeat until n is 1; if
// n reaches 1 (or is already 1 to begin with), then it must be divisible by 3.
//
// Complexity: probably runtime O(1) [ask the FPU], space O(1).
/*
class Solution {
    public boolean isPowerOfThree(final int n) {
        final var power = Math.log(n) / Math.log(3);
        return Math.abs(power - Math.rint(power)) < 1e-10;
    }
}
*/

// Alternative Solution: log10() method (base 10 log).
//
// Same concept as above, but log10() happens to not introduce rounding errors
// for log10(3), which allows it to correctly identify all powers of 3 within
// the range of a 32-bit int without needing an epsilon value for comparison.
//
// Complexity: probably runtime O(1) [ask the FPU], space O(1).
/*
class Solution {
    public boolean isPowerOfThree(final int n) {
        final var power = Math.log10(n) / Math.log10(3);
        return power % 1.0 == 0.0;
    }
}
*/
