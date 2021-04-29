// https://leetcode.com/problems/squares-of-a-sorted-array
//
// Complexity: runtime O(n), space O(1).

impl Solution {
    pub fn sorted_squares(mut nums: Vec<i32>) -> Vec<i32> {
        nums.sort_unstable_by_key(|x| x.abs());
        nums.iter_mut().for_each(|x| *x = *x * *x);
        nums
    }
}
