// https://leetcode.com/problems/longest-palindromic-substring
//
// Short Brute-Force Solution.
//
// Didn't have much time today, so I decided to try my luck with a super inefficient brute-force
// solution to this problem. Surprisingly, it passed!
//
// The "longest.len()" stuff in the loop ranges is to ensure we only try looking for palindromes
// that are longer than our current longest found.
//
// Complexity: runtime O(n^3), space O(1).

impl Solution {
    pub fn longest_palindrome(s: String) -> String {
        let mut longest = String::new();
        for i in 0..s.len() - longest.len() {
            for j in i + longest.len()..s.len() {
                let substr = &s[i..=j];
                if substr.chars().eq(substr.chars().rev()) {
                    longest = substr.to_owned();
                }
            }
        }

        longest
    }
}
