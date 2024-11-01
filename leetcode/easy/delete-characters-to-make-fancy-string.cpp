// https://leetcode.com/problems/delete-characters-to-make-fancy-string
// Complexity: runtime O(n), space O(n) [O(1) excluding output].

class Solution {
public:
    string makeFancyString(const string &s) const
    {
        string ret;
        ret.reserve(s.size());
        for (int chain = 0, i = 0; i < s.size(); ++i) {
            if (i == 0 || s[i - 1] == s[i])
                ++chain;
            else
                chain = 1;
            if (chain < 3)
                ret.push_back(s[i]);
        }
        return ret;
    }
};
