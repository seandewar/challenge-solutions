// https://leetcode.com/problems/first-unique-character-in-a-string/
//
// Uses a hash table and 2 traversals of the string. A lookup array containing
// all possible values for a string character can be used instead (and actually
// performs slightly better due to there only being 256 possibilities for a
// char).
//
// This can be done with only one traversal of the string if you care enough.
// If you instead store the index of the unique character (storing -1 if
// there's already a valid index there for non-uniques), you can traverse the
// map/array at the end instead to find out the index of the first unique
// character (e.g std::min_element()) and return it.
//
// Complexity: runtime O(n), space O(1) [specifically, O(UCHAR_MAX) - this can
// be changed to O(52) or O(26) if only considering alphabetical characters].

class Solution {
public:
    int firstUniqChar(string s)
    {
        unordered_map<char, unsigned> uniques;
        for (const char c : s)
            ++uniques[c];

        for (unsigned i{}; i < size(s); ++i) {
            if (uniques[s[i]] == 1)
                return i;
        }

        return -1;
    }
};
