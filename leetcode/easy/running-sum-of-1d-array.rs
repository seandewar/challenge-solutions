// https://leetcode.com/problems/running-sum-of-1d-array
//
// Complexity: runtime O(n), space O(1) [O(n) including output].

impl Solution {
    pub fn running_sum(nums: Vec<i32>) -> Vec<i32> {
        let mut res = Vec::with_capacity(nums.len());
        for i in 0..nums.len() {
            res.push(res.get(i - 1).unwrap_or(&0) + nums[i]);
        }
        res
    }
}
