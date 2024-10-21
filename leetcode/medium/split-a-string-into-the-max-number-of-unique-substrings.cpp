// https://leetcode.com/problems/split-a-string-into-the-max-number-of-unique-substrings
// Faster than 100% via checking if no better maximum is possible.
// Complexity: runtime O((2^n)*n) [2^n possible substring combinations, and
// string_view avoids the typical O(n) from copying to a new string allocation;
// however, hashing may take O(n) for each substring in the worst case, without
// using any tricks], space O(n).

class Solution {
    void findMax(const string_view s, unordered_set<string_view> &subs,
                 size_t &max_subs) const
    {
        if (s.empty()) {
            max_subs = max(max_subs, subs.size());
            return;
        }
        for (size_t len = 1; len <= s.size(); ++len) {
            const auto sub = s.substr(0, len), next_s = s.substr(len);
            if (subs.size() + next_s.size() < max_subs)
                return; // no better maximum possible
            if (!subs.insert(sub).second)
                continue;
            findMax(next_s, subs, max_subs);
            subs.erase(sub); // back-track
        }
    }

public:
    int maxUniqueSplit(const string &s) const
    {
        unordered_set<string_view> subs;
        size_t ret = 0;
        findMax(s, subs, ret);
        return static_cast<int>(ret);
    }
};
