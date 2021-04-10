// https://leetcode.com/problems/longest-increasing-path-in-a-matrix
//
// Uses dynamic programming to memoize and re-use previous longest increasing path size results.
// This solution can be made much shorter, but I wanted to use NonZeroI32 here :)
//
// Complexity: runtime O(m*n), space O(m*n).

use std::num::NonZeroI32;

impl Solution {
    pub fn longest_increasing_path(matrix: Vec<Vec<i32>>) -> i32 {
        let mut mem = vec![vec![None; matrix[0].len()]; matrix.len()];

        // flat_map() would be nice here, but we can't capture by move for the inner closure!
        (0..matrix.len())
            .map(|y| {
                (0..matrix[y].len())
                    .map(|x| Self::dfs(&matrix, &mut mem, x as _, y as _, -1))
                    .max()
                    .unwrap()
            })
            .max()
            .unwrap()
    }

    fn dfs(
        matrix: &Vec<Vec<i32>>,
        mem: &mut Vec<Vec<Option<NonZeroI32>>>,
        x: isize,
        y: isize,
        prev: i32,
    ) -> i32 {
        let (ux, uy) = (x as usize, y as usize);
        if let Some(row) = matrix.get(uy) {
            if let Some(&val) = row.get(ux) {
                if val > prev {
                    // borrow checker doesn't like get_or_insert_with() when using Option<_>
                    if mem[uy][ux].is_none() {
                        mem[uy][ux] = NonZeroI32::new(
                            [(0, 1), (0, -1), (1, 0), (-1, 0)]
                                .iter()
                                .map(|&(dx, dy)| Self::dfs(matrix, mem, x + dx, y + dy, val))
                                .max()
                                .unwrap()
                                + 1,
                        );
                    }

                    return mem[uy][ux].unwrap().get();
                }
            }
        }

        0
    }
}
