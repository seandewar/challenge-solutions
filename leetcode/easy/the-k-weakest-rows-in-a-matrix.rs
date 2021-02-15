// https://leetcode.com/problems/the-k-weakest-rows-in-a-matrix
//
// Complexity: runtime O(m*n+n*logn), space O(n).

impl Solution {
    pub fn k_weakest_rows(mat: Vec<Vec<i32>>, k: i32) -> Vec<i32> {
        // collect into a Vec of (row sum, row index) tuples
        let mut row_sums_to_idxs = mat
            .into_iter()
            .enumerate()
            .map(|(i, r)| (r.into_iter().sum::<i32>(), i))
            .collect::<Vec<_>>();

        // sort; if many rows have the same sum, they will instead be sorted by index, as Rust uses
        // a lexicographical ordering based on the order of tuple elements (which is why we have
        // the row index as the 2nd element in our tuples)
        row_sums_to_idxs.sort_unstable();

        // collect into a Vec of the k row indices to return
        row_sums_to_idxs
            .into_iter()
            .map(|(_, i)| i as i32)
            .take(k as _)
            .collect()
    }
}
