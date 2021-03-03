// https://leetcode.com/problems/missing-number
//
// XOR.
// Complexity: runtime O(n), space O(1).

impl Solution {
    pub fn missing_number(nums: Vec<i32>) -> i32 {
        nums.into_iter()
            .enumerate()
            .fold(0, |xor, (i, x)| xor ^ i as i32 + 1 ^ x)
    }
}

// Alternative Solution: Same as above, but avoids "i as i32 + 1".
// Complexity: runtime O(n), space O(1).

impl Solution {
    pub fn missing_number(nums: Vec<i32>) -> i32 {
        (1..=nums.len() as _)
            .zip(nums.into_iter())
            .fold(0, |xor, (x, y)| xor ^ x ^ y)
    }
}
