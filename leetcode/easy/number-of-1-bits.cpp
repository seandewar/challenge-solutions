// https://leetcode.com/problems/number-of-1-bits/
//
// Standard library solution using std::bitset.
// The number of set bits is also known as the Hamming Weight.
//
// Complexity: runtime O(1), space O(1).

class Solution {
public:
    int hammingWeight(const uint32_t n)
    {
        return bitset<32>{n}.count();
    }
};

// Alternative Solution: Without std::bitset.
// Complexity: runtime O(1), space O(1).
/*
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
*/
