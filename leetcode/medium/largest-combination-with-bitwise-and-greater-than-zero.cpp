// https://leetcode.com/problems/largest-combination-with-bitwise-and-greater-than-zero
// Complexity: runtime O(nlogx), space O(logx) [O(n), O(1) respectively due to
// constraints].

class Solution {
public:
    int largestCombination(const vector<int> &cands) const
    {
        array<int, (sizeof cands[0]) * CHAR_BIT> mask_freqs{};
        for (unsigned v : cands) {
            for (unsigned i = 0; v != 0 && i < mask_freqs.size();
                 v >>= 1, ++i) {
                if (v & 1)
                    ++mask_freqs[i];
            }
        }
        return *max_element(mask_freqs.begin(), mask_freqs.end());
    }
};
