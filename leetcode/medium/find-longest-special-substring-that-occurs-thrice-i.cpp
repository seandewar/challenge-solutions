// https://leetcode.com/problems/find-longest-special-substring-that-occurs-thrice-i
// Complexity: runtime O(n^2), space O(1) [O(26) due to constraints].

class Solution {
public:
    int maximumLength(const string &s) const
    {
        array<unsigned, 26> special_freqs;
        array<size_t, 26> c_freqs;
        for (auto n = s.size(); n > 0; --n) {
            special_freqs.fill(0);
            c_freqs.fill(0);
            bool special = false;
            for (size_t i = 0; i < n; ++i)
                special = ++c_freqs[s[i] - 'a'] == n;
            for (size_t l = 0; l <= s.size() - n; ++l) {
                if (special && ++special_freqs[s[l] - 'a'] == 3)
                    return n;
                if (l < s.size() - n) {
                    --c_freqs[s[l] - 'a'];
                    special = ++c_freqs[s[l + n] - 'a'] == n;
                }
            }
        }
        return -1;
    }
};
