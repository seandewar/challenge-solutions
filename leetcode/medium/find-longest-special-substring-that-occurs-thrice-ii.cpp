// https://leetcode.com/problems/find-longest-special-substring-that-occurs-thrice-ii
//
// Same problem as Find Longest Special Substring That Occurs Thrice I, but the
// constraints for s.size() is <= 5 * 10^5, so a O(n^2) solution doesn't pass.
//
// This works by observing that a special substring of size n also counts as
// 2 special substrings of size n - 1; for example, one "xxx" also counts as two
// "xx"s. This means we can simply find the longest sequences of repeating
// characters, then calculate their contribution to special substrings of
// smaller length.
//
// Complexity: runtime O(n), space O(n).

class Solution {
public:
    int maximumLength(const string &s) const
    {
        array<unordered_map<int, int>, 26> spec_freqs;
        array<int, 26> spec_max_lens{};
        for (size_t i = 0; i < s.size();) {
            const auto j = min(s.size(), s.find_first_not_of(s[i], i + 1));
            ++spec_freqs[s[i] - 'a'][j - i];
            spec_max_lens[s[i] - 'a'] =
                max<int>(spec_max_lens[s[i] - 'a'], j - i);
            i = j;
        }

        int ret = 0;
        for (int i = 0; i < 26; ++i) {
            int valid_spec_max_len = 0;
            const auto &freq_map = spec_freqs[i];
            for (int n = spec_max_lens[i], f = 0; n > valid_spec_max_len;
                 --n, ++f) {
                if (const auto it = freq_map.find(n); it != freq_map.end())
                    f += it->second;
                if (f >= 3) {
                    valid_spec_max_len = n;
                    break;
                }
            }
            ret = max(ret, valid_spec_max_len);
        }
        return ret > 0 ? ret : -1;
    }
};
