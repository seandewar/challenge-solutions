// https://leetcode.com/problems/wiggle-subsequence
//
// Greedy.
// Complexity: runtime O(n), space O(1).

impl Solution {
    pub fn wiggle_max_length(nums: Vec<i32>) -> i32 {
        nums[1..]
            .iter()
            .fold((nums[0], None, 1), |(prev, next_ord, res), &x| {
                if next_ord.map_or(x != prev, |o| o == prev.cmp(&x)) {
                    (x, Some(x.cmp(&prev)), res + 1)
                } else {
                    (x, next_ord, res)
                }
            })
            .2
    }
}
