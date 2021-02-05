// https://leetcode.com/problems/number-of-good-pairs
//
// A HashMap can be used instead for a O(n) runtime, if wanted.
// Complexity: runtime O(n^2), space O(1).

impl Solution {
    pub fn num_identical_pairs(nums: Vec<i32>) -> i32 {
        nums.iter().enumerate().fold(0, |acc, (i, &x)| {
            acc + &nums[i + 1..].iter().filter(|&&y| x == y).count()
        }) as _
    }
}
