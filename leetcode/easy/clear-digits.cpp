// https://leetcode.com/problems/clear-digits
// Complexity: runtime O(n), space O(n) including output, O(1) otherwise.

class Solution {
public:
    string clearDigits(const string &s) const
    {
        string ret;
        for (int skips = 0, i = s.size() - 1; i >= 0; --i) {
            if (s[i] >= '0' && s[i] <= '9') {
                ++skips;
                continue;
            }
            if (skips > 0) {
                --skips;
                continue;
            }
            ret.push_back(s[i]);
        }
        ranges::reverse(ret);
        return ret;
    }
};
