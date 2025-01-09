// https://leetcode.com/problems/counting-words-with-a-given-prefix
//
// I keep forgetting starts_with() exists and occasionally use mismatch()
// instead in my other solutions. >:(
//
// Complexity: runtime O(n*p), space O(1).

class Solution {
public:
    int prefixCount(const vector<string> &words, const string &pref) const
    {
        return count_if(words.begin(), words.end(),
                        [&](const auto &w) { return w.starts_with(pref); });
    }
};
