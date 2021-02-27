// https://leetcode.com/problems/xor-operation-in-an-array
//
// Complexity: runtime O(n), space O(1).

impl Solution {
    pub fn xor_operation(n: i32, start: i32) -> i32 {
        (0..n).fold(0, |acc, i| acc ^ (start + 2 * i))
    }
}
