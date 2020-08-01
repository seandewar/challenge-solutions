// https://leetcode.com/problems/detect-capital/
//
// Regex solution.
// Complexity: ideally runtime O(n).

class Solution {
public:
    bool detectCapitalUse(const string& word) const
    {
        // static so we don't need to re-compile the regex for each invocation
        static const regex reg("[A-Z]?([A-Z]*|[a-z]*)");
        return regex_match(word, reg);
    }
};

// Alternative Solution: Standard library solution.
// Complexity: runtime O(n), space O(1).

class Solution {
public:
    bool detectCapitalUse(const string& word) const
    {
        // invalid if we start with a lowercase but then have uppercases later
        if (islower(word[0])
            && any_of(cbegin(word) + 1, cend(word),
                      [](const char c) { return isupper(c); })) {
            return false;
        }

        // ensure all chars in word after the first have the same case
        return all_of(cbegin(word) + 1, cend(word), [&](const char c) {
            return !(isupper(word[1]) ^ isupper(c));
        });
    }
};
