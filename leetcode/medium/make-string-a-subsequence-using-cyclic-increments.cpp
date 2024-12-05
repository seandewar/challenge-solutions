// https://leetcode.com/problems/make-string-a-subsequence-using-cyclic-increments
// Complexity: runtime O(min(n,m)), space O(1).

class Solution {
public:
    bool canMakeSubsequence(const string &str1, const string &str2) const
    {
        for (size_t i = 0, j = 0;; ++i) {
            if (j == str2.size())
                return true;
            if (i + (str2.size() - j - 1) >= str1.size())
                return false;

            const char a = str1[i], b = str2[j];
            if (a == b || (a + 1 - 'a') % ('z' + 1 - 'a') + 'a' == b)
                ++j;
        }
    }
};
