// https://leetcode.com/problems/number-of-1-bits/
//
// The number of set bits is also known as the Hamming Weight.
// Complexity: runtime O(1), space O(1).

class Solution {
public:
    int hammingWeight(uint32_t n)
    {
        int result = 0;

        for (unsigned i = 0; i < 32; ++i) {
            result += n & 1;
            n >>= 1;
        } // alternative: result += (n >> i) & 1;

        return result;
    }
};
