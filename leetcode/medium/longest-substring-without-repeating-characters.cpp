// https://leetcode.com/problems/longest-substring-without-repeating-characters/
//
// Complexity: runtime O(n), space O(UCHAR_MAX) == O(1).

class Solution {
public:
    int lengthOfLongestSubstring(const string& s) const
    {
        unordered_set<char> seen;
        int result = 0;

        for (auto left = cbegin(s), right = cbegin(s);
             right != cend(s);
             ++right) {
            if (!seen.insert(*right).second) {
                while (*left != *right)
                    seen.erase(*(left++));
                ++left;
            }

            result = max<int>(result, size(seen));
        }

        return result;
    }
};
