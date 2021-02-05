// https://leetcode.com/problems/shuffle-the-array
//
// Complexity: runtime O(n), space O(n).

impl Solution {
    pub fn shuffle(nums: Vec<i32>, n: i32) -> Vec<i32> {
        nums.iter()
            .enumerate()
            .map(|(i, _)| nums[(i / 2) + (i % 2 * nums.len() / 2)])
            .collect()
    }
}
