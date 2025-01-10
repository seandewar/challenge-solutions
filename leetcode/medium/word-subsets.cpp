// https://leetcode.com/problems/word-subsets
// Complexity: runtime O(w1+w2), space O(1) [as we reuse the input storage;
// O(w1) otherwise, where w1 & w2 are the total number of characters in all
// words within words1 & words2 respectively].

class Solution {
public:
    vector<string> wordSubsets(vector<string> &words1,
                               const vector<string> &words2) const
    {
        array<int, 26> freqs2{};
        for (const auto &w : words2) {
            array<int, 26> freqs{};
            for (const auto c : w)
                ++freqs[c - 'a'];
            for (int i = 0; i < 26; ++i)
                freqs2[i] = max(freqs2[i], freqs[i]);
        }

        erase_if(words1, [&](const auto &w) {
            array<int, 26> freqs{};
            for (const auto c : w)
                ++freqs[c - 'a'];

            return !equal(freqs.begin(), freqs.end(), freqs2.begin(),
                          greater_equal{});
        });
        return words1;
    }
};
