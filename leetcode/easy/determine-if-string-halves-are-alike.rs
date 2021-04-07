// https://leetcode.com/problems/determine-if-string-halves-are-alike
//
// Complexity: runtime O(n), space O(1).

impl Solution {
    pub fn halves_are_alike(s: String) -> bool {
        let count = |s: &str| s.chars().filter(|&c| "aeiouAEIOU".contains(c)).count();
        let (l, r) = s.split_at(s.len() / 2);
        count(l) == count(r)
    }
}
