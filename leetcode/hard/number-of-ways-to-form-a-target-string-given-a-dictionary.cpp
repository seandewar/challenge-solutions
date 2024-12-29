// https://leetcode.com/problems/number-of-ways-to-form-a-target-string-given-a-dictionary
//
// Lots of hard-tier problems this December!
//
// In practice, indices are pretty densely distributed, so holding frequencies
// in a look-up array (one slot per discrete index) would be much more
// efficient, and allow us to improve the log term from the runtime complexity.
//
// Complexity: runtime O(tklogw), space O(tw).
// [where w = max_wn, k = sum of all word lengths, t = target length]

class Solution {
public:
    int numWays(const vector<string> &words, const string &target) const
    {
        unsigned max_wn = 0;
        array<map<int, unsigned>, 26> wmap;
        for (const auto &w : words) {
            for (int i = 0; i < w.size(); ++i)
                ++wmap[w[i] - 'a'][i];
            max_wn = max<unsigned>(max_wn, w.size());
        }
        if (max_wn < target.size())
            return 0;

        constexpr auto mod = static_cast<int>(1e9) + 7;
        vector memo(max_wn * target.size(), -1);
        const auto dfs = [&](this const auto &self, const int ti,
                             const int wi) -> int {
            if (ti >= target.size())
                return 1;
            if (max_wn - wi < target.size() - ti)
                return 0;

            auto &mem = memo[ti * max_wn + wi];
            if (mem != -1)
                return mem;

            const char c = target[ti];
            const auto &wis = wmap[c - 'a'];
            uint_least64_t ret = 0;
            for (auto it = wis.lower_bound(wi); it != wis.end(); ++it) {
                ret += static_cast<uint_least64_t>(it->second)
                       * self(ti + 1, it->first + 1);
                ret %= mod;
            }
            return mem = ret;
        };
        return dfs(0, 0);
    }
};
