// https://leetcode.com/problems/number-of-submatrices-that-sum-to-target
//
// This can be optimized with a bit of dynamic programming, but this still passes:
//
// Calculate the 2D cumulative (prefix) sum of all sub-matrices with bottom-right corners (x, y)
// for all x and y. Try every sub-matrix using this information.
//
// We will have to take special care in considering cases such as this:
// c c a a a
// b b 8 9 0  where we want to consider the sub-matrix: 8 9 0
// b b 3 4 5                                            3 4 5 [bottom-right at (4, 2)]
//
// To get the sum of that sub-matrix, we'll need to get the cumulative sum at (4, 2), minus the
// cumulative sums at (4, 0) and (1, 2) for regions a and b respectively.
//
// Because a & b overlaps at region c [bottom-right (2, 0)], we'll need to add the cumulative sum
// of that region, as it would be subtracted twice due to the overlap.
//
// Complexity: runtime O((m*n)^2), space O(1).

impl Solution {
    pub fn num_submatrix_sum_target(mut matrix: Vec<Vec<i32>>, target: i32) -> i32 {
        let (m, n, mut res) = (matrix.len(), matrix[0].len(), 0);
        for y in 0..m {
            let mut row_sum = 0;
            for x in 0..n {
                matrix[y][x] += row_sum;
                row_sum = matrix[y][x];
                matrix[y][x] += if y > 0 { matrix[y - 1][x] } else { 0 };
            }
        }

        for y in 0..m {
            for x in 0..n {
                for y2 in 0..=y {
                    res += (0..=x)
                        .filter(|&x2| {
                            let col_sum = if x2 != x { matrix[y][x2] } else { 0 };
                            let row_sum = if y2 != y { matrix[y2][x] } else { 0 };
                            let overlap_sum = if x2 != x && y2 != y { matrix[y2][x2] } else { 0 };
                            matrix[y][x] - col_sum - row_sum + overlap_sum == target
                        })
                        .count();
                }
            }
        }

        res as _
    }
}
