// https://leetcode.com/problems/reverse-string-ii/
//
// Complexity: runtime O(n), space O(1).

class Solution {
public:
    string reverseStr(string s, int k)
    {
        for (int i = 0; i < size(s); i += 2 * k) {
            const auto first = begin(s) + i;
            reverse(first, min(end(s), first + k));
        }

        return s;
    }
};
