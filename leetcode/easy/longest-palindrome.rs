// https://leetcode.com/problems/longest-palindrome/
//
// Complexity: runtime O(n), space O(n).

impl Solution {
    pub fn longest_palindrome(s: String) -> i32 {
        let mut unmatched = std::collections::HashSet::new();
        let mut pairs = 0;

        for c in s.chars() {
            if !unmatched.insert(c) {
                unmatched.remove(&c);
                pairs += 1;
            }
        }

        pairs * 2 + if !unmatched.is_empty() { 1 } else { 0 }
    }
}
