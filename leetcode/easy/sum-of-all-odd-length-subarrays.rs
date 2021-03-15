// https://leetcode.com/problems/sum-of-all-odd-length-subarrays
//
// Complexity: runtime O(n^2), space O(1).

impl Solution {
    pub fn sum_odd_length_subarrays(arr: Vec<i32>) -> i32 {
        (1..=arr.len())
            .step_by(2)
            .map(|n| arr.windows(n).map(|s| s.iter().sum::<i32>()).sum::<i32>())
            .sum()
    }
}
