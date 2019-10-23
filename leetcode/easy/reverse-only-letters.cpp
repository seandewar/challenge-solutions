// https://leetcode.com/problems/reverse-only-letters/
//
// Complexity: runtime O(n) (specifically, <= O(n/2)), space O(1).

class Solution {
public:
    string reverseOnlyLetters(string s)
    {
        if (empty(s)) {
            return s;
        }

        auto front = begin(s);
        auto back = end(s) - 1;

        while (front < back) {
            if (!isalpha(*front)) {
                ++front;
            }
            else if (!isalpha(*back)) {
                --back;
            }
            else {
                swap(*front++, *back--);
            }
        }

        return s;
    }
};
