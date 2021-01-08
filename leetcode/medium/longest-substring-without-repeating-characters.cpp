// https://leetcode.com/problems/longest-substring-without-repeating-characters/
//
// Complexity: runtime O(n), space O(UCHAR_MAX) == O(1).

class Solution {
public:
    int lengthOfLongestSubstring(const string& s) const
    {
        size_t left = 0, right = 0, maxLen = 0;
        unordered_set<char> substrSet; // could use an array instead

        while (right < size(s)) {
            if (substrSet.insert(s[right]).second) {
                maxLen = max(maxLen, size(substrSet));
                ++right;
            } else { // unsuccessful insertion; char already in the set
                substrSet.erase(s[left++]);
            }
        }

        return maxLen;
    }
};
