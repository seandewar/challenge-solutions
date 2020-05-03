// https://leetcode.com/problems/ransom-note/
//
// Hash map solution.
// Complexity: runtime O(m+n), space O(m) [where n = size(ransomNote),
//                                               m = size(magazine)]

class Solution {
public:
    bool canConstruct(const string& ransomNote, const string& magazine) const {
        unordered_map<char, string::size_type> charCounts;
        for (const char c : magazine)
            ++charCounts[c];

        for (const char c : ransomNote) {
            if (charCounts[c]-- == 0) // underflows; won't affect result
                return false;
        }

        return true;
    }
};

// Alternative Solution: Using an array.
// Slightly faster as it avoids hash table overhead, but same complexity.
//
// Complexity: runtime O(m+n), space O(m) [where n = size(ransomNote),
//                                               m = size(magazine)]

class Solution {
public:
    bool canConstruct(const string& ransomNote, const string& magazine) const {
        // test input guaranteed to only contain lower case letters
        array<string::size_type, 26> charCounts{};
        for (const char c : magazine)
            ++charCounts[c - 'a']; // not technically portable according to the
                                   // standard (ordering only guaranteed for
                                   // 0-9), but will work in pretty much all
                                   // environments (most use ascii or unicode)

        for (const char c : ransomNote) {
            if (charCounts[c - 'a']-- == 0) // underflows; won't affect result
                return false;
        }

        return true;
    }
};
