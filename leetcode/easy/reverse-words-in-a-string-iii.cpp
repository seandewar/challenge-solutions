// https://leetcode.com/problems/reverse-words-in-a-string-iii
// Complexity: runtime O(n), space O(1).

class Solution {
public:
    string reverseWords(string s)
    {
        for (string::size_type i = 0; i < s.size();) {
            auto end_i = min(s.size(), s.find(' ', i));
            reverse(s.begin() + i, s.begin() + end_i);
            i = end_i + 1;
        }
        return s;
    }
};
