// https://leetcode.com/problems/how-many-numbers-are-smaller-than-the-current-number
//
// Complexity: runtime O(n^2), space O(1) [O(n) including output].

impl Solution {
    pub fn smaller_numbers_than_current(nums: Vec<i32>) -> Vec<i32> {
        nums.iter()
            .map(|&x| nums.iter().filter(|&&y| x > y).count() as i32)
            .collect()
    }
}
