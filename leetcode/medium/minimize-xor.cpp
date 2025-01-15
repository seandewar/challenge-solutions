// https://leetcode.com/problems/minimize-xor
// Complexity: runtime O(logn), space O(1).

class Solution {
public:
    unsigned minimizeXor(const unsigned num1, const unsigned num2) const
    {
        unsigned rem_pop = popcount(num2), ret = 0;
        for (auto mask = 1u << (CHAR_BIT * sizeof ret - 1); mask && rem_pop;
             mask >>= 1) {
            if (mask & num1) {
                ret |= mask;
                --rem_pop;
            }
        }
        for (auto mask = 1u; rem_pop; mask <<= 1) {
            assert(mask); // num2 shouldn't have too many set bits...
            if (!(mask & ret) && !(mask & num1)) {
                ret |= mask;
                --rem_pop;
            }
        }

        return ret;
    }
};
