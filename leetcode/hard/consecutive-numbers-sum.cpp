// https://leetcode.com/problems/consecutive-numbers-sum
//
// This one took me quite a while; the first thing that popped into my head was
// the summation formula, but I got quite stuck on figuring out how it could be
// applied to count the number of valid sums...
//
// Anyway, we can express the consecutive sums that make up N (our input) as:
// N  = x  + (x + 1) + (x + 2) + .. + (x + a - 1)
//    = ax + 1       + 2       + .. + (a - 1)
//
// Using the summation formula: 1 + 2 + .. + n = n * (n + 1) / 2
// We can instead express N as:
// N  = ax + (a - 1) * a / 2
//    = ax + (a^2 - a) / 2
//
// Now we just need to solve for ax:
// ax = N - (a^2 - a) / 2
//
// We can try every integer from 1 to N as "a" here; a valid sequence exists if
// x is also an integer (>= 1), so we just need to check that ax is divisible by
// a, and bail if ax reaches <= 0.
//
// We'll only end up trying at most sqrt(N) numbers, as:
// 0  < N - (a^2 - a) / 2
// -N < - (a^2 - a)
// N  > a^2 - a
// Which is approximately: a < sqrt(N)
//
// Complexity: runtime O(sqrt(n)), space O(1).

class Solution {
public:
    int consecutiveNumbersSum(const int n) const noexcept {
        int ret = 0;
        for (int a = 1; a <= n; ++a) {
            const int twoax = 2 * n - (a * a - a);
            if (twoax <= 0)
                break;
            if (twoax % 2 == 0 && (twoax / 2) % a == 0)
                ++ret;
        }
        return ret;
    }
};
