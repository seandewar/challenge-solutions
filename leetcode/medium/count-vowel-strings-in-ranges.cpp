// https://leetcode.com/problems/count-vowel-strings-in-ranges
// Complexity: runtime O(n+q), space O(n) [excluding output storage].

class Solution {
public:
    vector<int> vowelStrings(const vector<string> &words,
                             const vector<vector<int>> &queries) const
    {
        vector<int> prefixes, ret;
        prefixes.reserve(words.size() + 1);
        prefixes.push_back(0);
        constexpr array vowels{'a', 'e', 'i', 'o', 'u'};
        for (int prefix = 0; const auto &w : words) {
            if (find(vowels.begin(), vowels.end(), w.front()) != vowels.end()
                && find(vowels.begin(), vowels.end(), w.back()) != vowels.end())
                ++prefix;
            prefixes.push_back(prefix);
        }

        ret.reserve(queries.size());
        for (const auto &q : queries)
            ret.push_back(prefixes[q[1] + 1] - prefixes[q[0]]);
        return ret;
    }
};
