// https://leetcode.com/problems/rotate-image
//
// Using std::mem::replace().
// Complexity: runtime O(n*n), space O(1).

use std::mem::replace;

impl Solution {
    pub fn rotate(matrix: &mut Vec<Vec<i32>>) {
        let n = matrix.len();
        for layer in 0..(n + 1) / 2 {
            for i in layer..n - layer - 1 {
                let mut v = matrix[layer][i];
                v = replace(&mut matrix[i][n - layer - 1], v);
                v = replace(&mut matrix[n - layer - 1][n - i - 1], v);
                matrix[layer][i] = replace(&mut matrix[n - i - 1][layer], v);
            }
        }
    }
}
