// https://leetcode.com/problems/get-maximum-in-generated-array/
//
// Complexity: runtime O(n), space O(n).

impl Solution {
    pub fn get_maximum_generated(n: i32) -> i32 {
        let mut nums = Vec::with_capacity(n as usize + 1);
        for i in 0..=n as usize {
            nums.push(match i {
                i if i < 2 => i as i32,
                i if i % 2 == 0 => nums[i / 2],
                _ => nums[i / 2] + nums[i / 2 + 1],
            });
        }

        nums.into_iter().max().unwrap_or(0)
    }
}
