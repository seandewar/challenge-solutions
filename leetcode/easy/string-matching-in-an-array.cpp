// https://leetcode.com/problems/string-matching-in-an-array
//
// Many different ways to do this (maps, tries, etc.), but let's just sort,
// de-duplicate and binary search. We need only push words[i].size() suffixes,
// as binary searching will find any such substring that prefixes it; we need
// only check for a mismatch within the prefix.
//
// Complexity: runtime O(plogp), space O(p) [where p == sum of word lengths;
// ignoring output].

class Solution {
public:
    vector<string> stringMatching(const vector<string> &words) const
    {
        vector<string_view> suffixes;
        for (const auto &w : words)
            for (size_t i = 0; i < w.size(); ++i)
                suffixes.emplace_back(w.data() + i, w.size() - i - (i == 0));

        sort(suffixes.begin(), suffixes.end());
        suffixes.erase(unique(suffixes.begin(), suffixes.end()),
                       suffixes.end());

        vector<string> ret;
        for (const auto &w : words) {
            const auto it = lower_bound(suffixes.begin(), suffixes.end(), w);
            if (it != suffixes.end()
                && mismatch(it->begin(), it->end(), w.begin()).second
                       == w.end())
                ret.push_back(w);
        }
        return ret;
    }
};
