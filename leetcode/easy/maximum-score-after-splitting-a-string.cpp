// https://leetcode.com/problems/maximum-score-after-splitting-a-string
// Complexity: runtime O(n), space O(1).

class Solution {
public:
    int maxScore(const string &s) const
    {
        int ret = 0;
        for (int r = 1, lzeroes = s[0] == '0',
                 rones = count(s.begin() + r, s.end(), '1');
             r < s.size(); ++r) {
            ret = max(ret, lzeroes + rones);
            lzeroes += s[r] == '0';
            rones -= s[r] == '1';
        }
        return ret;
    }
};
