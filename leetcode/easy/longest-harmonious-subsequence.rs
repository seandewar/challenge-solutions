// https://leetcode.com/problems/longest-harmonious-subsequence
//
// Complexity: runtime O(n), space O(n).

impl Solution {
    pub fn find_lhs(nums: Vec<i32>) -> i32 {
        let mut counts = std::collections::HashMap::new();
        nums.into_iter()
            .for_each(|x| *counts.entry(x).or_insert(0) += 1);

        counts
            .iter()
            .filter_map(|(&k, &v)| counts.get(&(k + 1)).map(|n| n + v))
            .max()
            .unwrap_or(0)
    }
}
