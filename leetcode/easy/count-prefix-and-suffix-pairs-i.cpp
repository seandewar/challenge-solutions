// https://leetcode.com/problems/count-prefix-and-suffix-pairs-i
// 100 day daily problem streak!
// Complexity: runtime O(n^2*m), space O(1) [where n = words.size(),
//                                                 m = words[i].size()]

class Solution {
public:
    int countPrefixSuffixPairs(const vector<string> &words) const
    {
        int ret = 0;
        for (int i = 0; i < words.size(); ++i) {
            const auto &w1 = words[i];
            for (int j = i + 1; j < words.size(); ++j) {
                const auto &w2 = words[j];
                if (mismatch(w1.begin(), w1.end(), w2.begin()).first == w1.end()
                    && mismatch(w1.rbegin(), w1.rend(), w2.rbegin()).first
                           == w1.rend())
                    ++ret;
            }
        }
        return ret;
    }
};
