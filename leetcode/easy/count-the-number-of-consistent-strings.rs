// https://leetcode.com/problems/count-the-number-of-consistent-strings
//
// Complexity: runtime O(a+w*l), space O(a). [where a == allowed.len(), w == words.len(), l ==
// average word len]

impl Solution {
    pub fn count_consistent_strings(allowed: String, words: Vec<String>) -> i32 {
        let allowed_set: std::collections::HashSet<_> = allowed.chars().collect();
        words
            .into_iter()
            .filter(|w| w.chars().all(|c| allowed_set.contains(&c)))
            .count() as _
    }
}
