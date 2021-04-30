// https://leetcode.com/problems/valid-mountain-array
//
// Complexity: runtime O(n), space O(1).

impl Solution {
    pub fn valid_mountain_array(arr: Vec<i32>) -> bool {
        (1..arr.len())
            .find(|&i| arr[i - 1] >= arr[i])
            .map_or(false, |j| {
                j > 1 && (j..arr.len()).all(|j| arr[j - 1] > arr[j])
            })
    }
}
