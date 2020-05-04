// https://leetcode.com/problems/number-complement/
//
// Complexity: runtime O(sizeof(int) * CHAR_BIT) == O(1), space O(1).

class Solution {
public:
    int findComplement(const int num) const
    {
        auto mask = ~0u; // -1u, numeric_limits<unsigned>::max(), UINT_MAX...

        while (mask & num)
            mask <<= 1;

        return ~num & ~mask;
    }
};
