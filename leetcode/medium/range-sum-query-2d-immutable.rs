// https://leetcode.com/problems/range-sum-query-2d-immutable
//
// Converting each row in the matrix into a row of prefix sums of values in that row is enough.
// Complexity: runtime new(): O(w*h), sum_region(): O(h),
//             space O(1), as the input matrix is converted in-place and moved into our struct.
//             [where w & h is the width & height of the input matrix respectively]

struct NumMatrix {
    mat: Vec<Vec<i32>>,
}

impl NumMatrix {
    fn new(mut mat: Vec<Vec<i32>>) -> Self {
        mat.iter_mut()
            .for_each(|row| (1..row.len()).for_each(|i| row[i] += row[i - 1]));

        Self { mat }
    }

    fn sum_region(&self, row1: i32, col1: i32, row2: i32, col2: i32) -> i32 {
        let (row1, row2) = (row1 as usize, row2 as usize);
        let (col1, col2) = (col1 as usize, col2 as usize);
        (row1..=row2)
            .map(|y| self.mat[y][col2] - self.mat[y].get(col1 - 1).unwrap_or(&0))
            .sum()
    }
}
