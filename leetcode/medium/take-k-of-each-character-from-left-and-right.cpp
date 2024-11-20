// https://leetcode.com/problems/take-k-of-each-character-from-left-and-right
// Complexity: runtime O(n), space O(1).

class Solution {
public:
    int takeCharacters(const string &s, const int k) const
    {
        array<int, 3> freqs{};
        const auto is_valid = [&] {
            return all_of(freqs.begin(), freqs.end(),
                          [k](const int freq) { return freq >= k; });
        };

        int l = 0;
        while (!is_valid() && l < s.size())
            ++freqs[s[l++] - 'a'];
        if (!is_valid())
            return -1;

        int ret = l;
        for (int r = 0; l > 0; ret = min(ret, l + r)) {
            --freqs[s[--l] - 'a'];
            while (!is_valid())
                ++freqs[s[s.size() - ++r] - 'a'];
        }
        return ret;
    }
};
