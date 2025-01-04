// https://leetcode.com/problems/unique-length-3-palindromic-subsequences
// Binary searching for the middle character made this faster than 100%.
// Complexity: runtime O(n), space O(n).

class Solution {
public:
    int countPalindromicSubsequence(const string &s) const
    {
        array<vector<int>, 26> is;
        for (int i = 0; i < s.size(); ++i)
            is[s[i] - 'a'].push_back(i);

        int ret = 0;
        for (const auto &v1 : is) {
            if (v1.size() < 2)
                continue;

            const auto l = v1.front(), r = v1.back();
            if (r <= l + 1)
                continue;

            for (const auto &v2 : is) {
                const auto it = upper_bound(v2.begin(), v2.end(), l);
                if (it == v2.end() || *it >= r)
                    continue;

                ++ret;
            }
        }
        return ret;
    }
};
