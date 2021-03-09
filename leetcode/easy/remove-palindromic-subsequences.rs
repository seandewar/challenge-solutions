// https://leetcode.com/problems/remove-palindromic-subsequences
//
// There are only 3 cases we need to consider:
//
// Empty string: 0 operations are needed.
//
// Whole string is palindromic: 1 operation is needed.
//
// Other cases: 2 operations are needed:
//
//   As there are only 2 letters to consider, a & b, we can, for example, simply skip all as, which
//   leaves us with only bs.
//
//   This string of bs is palindromic, which is true for any string that contains only 1 type of
//   letter, so we can delete the bs.
//
//   Afterwards, we can delete the remaining as via the same logic, or vice versa.
//
// Complexity: runtime O(n), space O(1).

impl Solution {
    pub fn remove_palindrome_sub(s: String) -> i32 {
        if s.is_empty() {
            0
        } else if s.chars().eq(s.chars().rev()) {
            1
        } else {
            2
        }
    }
}

// Alternative Solution: Optimized version of the above which only tests the 2 halves of the string
// for the whole string palindromic check.
//
// Complexity: runtime O(n), space O(1).

impl Solution {
    pub fn remove_palindrome_sub(s: String) -> i32 {
        if s.is_empty() {
            0
        } else if s
            .chars()
            .take(s.len() / 2)
            .eq(s.chars().rev().take(s.len() / 2))
        {
            1
        } else {
            2
        }
    }
}
