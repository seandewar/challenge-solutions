// https://leetcode.com/problems/maximum-repeating-substring
//
// Complexity: runtime O(n^2), space O(n).

impl Solution {
    pub fn max_repeating(sequence: String, word: String) -> i32 {
        let max_word = word.repeat(sequence.len() / word.len());
        let mut n = max_word.len();
        while sequence.find(&max_word[..n]).is_none() {
            n -= word.len();
        }
        (n / word.len()) as _
    }
}
