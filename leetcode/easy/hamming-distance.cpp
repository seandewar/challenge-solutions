// https://leetcode.com/problems/hamming-distance/
//
// The number of differently set bits between 2 binary values is known as the
// Hamming Distance.
// Complexity: runtime O(1), space O(1).

class Solution {
public:
    int hammingDistance(int x, int y)
    {
        unsigned setBits{x ^ y};
        int result{};

        // alternative: i < numeric_limits<unsigned>::digits, assuming
        // numeric_limits<unsigned>::radix == 2 (uses binary digits)
        for (unsigned i{}; i < sizeof(setBits) * CHAR_BIT; ++i) {
            result += setBits & 1;
            setBits >>= 1;
        } // alternative: result += (setBits >> i) & 1;

        return result;
    }
};
