// https://leetcode.com/problems/check-if-two-string-arrays-are-equivalent/
//
// Iterator solution.
//
// Complexity: runtime O(min(n,m)), space O(1). [where n, m = number of chars in word1, word2]

impl Solution {
    pub fn array_strings_are_equal(word1: Vec<String>, word2: Vec<String>) -> bool {
        word1
            .iter()
            .flat_map(|s| s.chars())
            .eq(word2.iter().flat_map(|s| s.chars()))
    }
}

// Alternative Solution: Using Vec::concat().
// Complexity: runtime O(n+m), space O(n+m). [where n, m = number of chars in word1, word2]

impl Solution {
    pub fn array_strings_are_equal(word1: Vec<String>, word2: Vec<String>) -> bool {
        word1.concat() == word2.concat()
    }
}
