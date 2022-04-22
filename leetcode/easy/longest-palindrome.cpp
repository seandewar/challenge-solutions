// https://leetcode.com/problems/longest-palindrome
//
// Complexity: runtime O(n), space O(1).

class Solution {
public:
    int longestPalindrome(const string s) const noexcept
    {
        array<string::size_type, 'z' - 'A' + 1> counts{};
        for (const char c : s)
            ++counts[c - 'A'];

        return accumulate(cbegin(counts), cend(counts), 0,
                          [](const auto a, const auto b) noexcept {
                              return a + b - b % 2;
                          })
               + any_of(cbegin(counts), cend(counts),
                        [](const auto a) noexcept { return a % 2; });
    }
};
