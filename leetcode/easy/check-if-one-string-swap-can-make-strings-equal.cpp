// https://leetcode.com/problems/check-if-one-string-swap-can-make-strings-equal
// Complexity: runtime O(n), space O(1) ["O(26)"].

class Solution {
public:
    bool areAlmostEqual(const string &s1, const string &s2) const
    {
        array<int, 26> freqs{};
        for (const char c : s1)
            ++freqs[c - 'a'];
        for (const char c : s2)
            --freqs[c - 'a'];

        if (ranges::any_of(freqs, [](const int f) { return f != 0; }))
            return false;

        const auto [it1_s1, it1_s2] = ranges::mismatch(s1, s2);
        if (it1_s1 == s1.end())
            return true;

        const auto [it2_s1, it2_s2] =
            mismatch(it1_s1 + 1, s1.end(), it1_s2 + 1);
        if (it2_s1 == s1.end())
            return false;

        return mismatch(it2_s1 + 1, s1.end(), it2_s2 + 1).first == s1.end();
    }
};
