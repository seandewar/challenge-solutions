// https://leetcode.com/problems/search-a-2d-matrix-ii
//
// Probably over-engineered, but this solution works by bisecting from (0, 0) row-first, then the
// column of the lower-bound after if the target is not found, repeating this process until the
// target is found somewhere.
//
// To avoid getting stuck in an infinite loop, if we end up searching a row/column we have already
// bisected before, we instead continue from the position immediately before we last searched
// there; if there is no such position, then the matrix doesn't contain the target, and we can
// abort.
//
// For example, given the target 11 and the matrix:
// 1  4  7
// 2  11 12
// 3  13 14
//
// We bisect row 0. The lower bound is 7 (2, 0).
// We bisect column 2. The lower bound is still 7 (2, 0).
// To avoid an infinite loop, we bisect the column before: column 1. The lower bound is 11 (1, 1).
// 11 is our target so we return true.
//
// Now, for a target that doesn't exist within the matrix: 10:
//
// We bisect row 0. The lower bound is 7 (2, 0).
// We bisect column 2. The lower bound is still 7 (2, 0).
// We bisect column 1. The lower bound is 4 (1, 0).
// We bisect column 0. The lower bound is 3 (0, 2).
// We bisect row 2. The lower bound is still 3 (2, 0).
// We have nowhere else to go. Target isn't in the matrix; abort!
//
// No idea if this is more efficient than just binary searching every row/column.
//
// Complexity: runtime: no idea, but it's faster than ~100% most times, space O(m+n).

impl Solution {
    pub fn search_matrix(mut matrix: Vec<Vec<i32>>, target: i32) -> bool {
        let (m, n) = (matrix.len(), matrix[0].len());
        let (mut row_upper_bounds, mut col_upper_bounds) = (vec![n; m], vec![m; n]);
        let (mut x, mut y) = (0, 0);
        loop {
            x = match row_upper_bounds[y] {
                0 => {
                    return false;
                }
                b if b == n => match matrix[y].binary_search(&target) {
                    Ok(_) => {
                        return true;
                    }
                    Err(0) => {
                        return false;
                    }
                    Err(i) => i - 1,
                },
                b => b - 1,
            };
            row_upper_bounds[y] = x;

            y = match col_upper_bounds[x] {
                0 => {
                    return false;
                }
                b if b == m => match matrix.binary_search_by(|row| row[x].cmp(&target)) {
                    Ok(_) => {
                        return true;
                    }
                    Err(0) => {
                        return false;
                    }
                    Err(i) => i - 1,
                },
                b => b - 1,
            };
            col_upper_bounds[x] = y;
        }
    }
}
