// https://leetcode.com/problems/reverse-string/
//
// Swaps characters in-place. Equivalent to:
//
// auto left = begin(s), right = end(s);
// while (left != right && left != --right)
//     iter_swap(left++, right);
//
// (while loop's condition handles empty(s) where left==right==end(s) & where
//  size(s) is even such that when left & right are next to each other near the
//  centre of s, they don't surpass each other without equalling each other in
//  the next iteration)
//
// Complexity: runtime O(n), space O(1).

class Solution {
public:
    void reverseString(vector<char>& s)
    {
        reverse(begin(s), end(s));
    }
};
