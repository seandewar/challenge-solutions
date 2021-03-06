// https://leetcode.com/problems/short-encoding-of-words
//
// Complexity: runtime O(n*w^2), space O(n*w^2).

impl Solution {
    pub fn minimum_length_encoding(words: Vec<String>) -> i32 {
        let mut ignore: std::collections::HashSet<_> = words
            .iter()
            .flat_map(|w| w.char_indices().skip(1).map(move |(i, _)| &w[i..]))
            .collect();
        words
            .iter()
            .filter_map(|w| Some(w.len() as i32 + 1).filter(|_| ignore.insert(w)))
            .sum()
    }
}
