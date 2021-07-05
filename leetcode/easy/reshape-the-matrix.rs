// https://leetcode.com/problems/reshape-the-matrix
//
// Complexity: runtime O(m*n), space O(m*n).

impl Solution {
    pub fn matrix_reshape(mat: Vec<Vec<i32>>, r: i32, c: i32) -> Vec<Vec<i32>> {
        let (r, c) = (r as usize, c as usize);
        let (m, n) = (mat.len(), mat[0].len());
        if r * c != m * n {
            mat
        } else {
            mat.iter()
                .flat_map(|r| r.iter().copied())
                .collect::<Vec<_>>()
                .chunks(c)
                .map(|r| r.to_vec())
                .collect()
        }
    }
}
