// https://leetcode.com/problems/valid-palindrome/
//
// Uses a sliding window.
// Complexity: runtime O(n), space O(1).

class Solution {
    public boolean isPalindrome(final String s) {
        int left = 0, right = s.length() - 1;

        while (left < right) {
            if (!Character.isLetterOrDigit(s.charAt(left)))
                ++left;
            else if (!Character.isLetterOrDigit(s.charAt(right)))
                --right;
            else if (Character.toLowerCase(s.charAt(left++))
                    != Character.toLowerCase(s.charAt(right--)))
                return false;
        }

        return true;
    }
}
