// https://leetcode.com/problems/smallest-number-with-all-set-bits
// Complexity: runtime O(logx), space O(1).

class Solution {
public:
    int smallestNumber(const int n) const
    {
        unsigned i = 0;
        for (unsigned x = n; x; x >>= 1, ++i) {}
        return (1 << i) - 1;
    }
};
