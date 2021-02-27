// https://leetcode.com/problems/create-target-array-in-the-given-order
//
// Complexity: runtime O(n^2), space O(n).

impl Solution {
    pub fn create_target_array(nums: Vec<i32>, index: Vec<i32>) -> Vec<i32> {
        let mut res = Vec::with_capacity(nums.len());
        for (i, v) in index.into_iter().zip(nums) {
            res.insert(i as _, v);
        }

        res
    }
}
