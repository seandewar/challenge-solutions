// https://leetcode.com/problems/factorial-trailing-zeroes/
//
// How many times n! divides evenly by 10 will determine the number of trailing
// zeroes. This depends on how many multiples of 5 exist from 1 to n, as due to
// the nature of factorials, each multiple of 5 will be preceded by some
// multiple of 2, producing factors 5 & 2 such that 5*2==10 (e.g: 5! ==
// (5)*[4==2*2]*3*[2]*1), which is required to create a trailing zero.
//
// Note that this means we will need to account for the extra trailing zeroes in
// cases where n is >= some square of 5 that is > 5^1==5 itself, as these
// squares will, by definition, have numerous 5s as their factors (e.g: 25! ==
// (25==5*5)*...*3*[2]*1).
//
// Complexity: runtime O(logn), space O(1).

class Solution {
public:
    int trailingZeroes(int n) const
    {
        int result = 0;

        while (n != 0)
            result += n /= 5;

        return result;
    }
};

// Alternative Solution: recursion.
// Complexity: runtime O(logn), space O(logn).
/*
class Solution {
public:
    int trailingZeroes(const int n) const
    {
        return n == 0 ? 0 : n / 5 + trailingZeroes(n / 5);
    }
};
*/
