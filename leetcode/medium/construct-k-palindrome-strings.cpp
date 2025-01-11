// https://leetcode.com/problems/construct-k-palindrome-strings
// Complexity: runtime O(n), space O(1).

class Solution {
public:
    bool canConstruct(const string &s, const int k) const
    {
        if (k >= s.size())
            return k == s.size();

        array<bool, 26> is_single{};
        for (const char c : s)
            is_single[c - 'a'] = !is_single[c - 'a'];

        return ranges::count(is_single, true) <= k;
    }
};
