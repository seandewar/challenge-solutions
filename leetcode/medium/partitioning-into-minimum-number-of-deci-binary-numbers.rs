// https://leetcode.com/problems/partitioning-into-minimum-number-of-deci-binary-numbers
//
// Complexity: runtime O(n), space O(1).

impl Solution {
    pub fn min_partitions(n: String) -> i32 {
        n.chars()
            .map(|c| c.to_digit(10).unwrap() as _)
            .max()
            .unwrap()
    }
}
