// https://leetcode.com/problems/n-queens
//
// Back-tracking + Recursion.
// This has room for optimization.
// (Based on my N-Queens I Solution)
//
// Complexity: runtime O(n^3), space O(n^2).

impl Solution {
    pub fn total_n_queens(n: i32) -> i32 {
        let mut board = vec![vec![false; n as _]; n as _];
        Self::dfs(&mut board, n as _, 0) as _
    }

    fn dfs(board: &mut Vec<Vec<bool>>, queens: usize, from_y: usize) -> usize {
        if queens == 0 {
            1
        } else {
            let (n, mut res) = (board.len(), 0);
            for y in from_y..n {
                for x in 0..n {
                    if Self::safe_square(board, x, y) {
                        board[y][x] = true;
                        res += Self::dfs(board, queens - 1, y + 1); // can skip this row
                        board[y][x] = false; // back-track
                    }
                }
            }
            res
        }
    }

    fn safe_square(board: &Vec<Vec<bool>>, x: usize, y: usize) -> bool {
        let n = board.len();
        let safe = |x: usize, y: usize| x >= n || y >= n || !board[y][x];
        (0..n).all(|i| {
            safe(i, y)
                && safe(x, i)
                && safe(x - i, y - i)
                && safe(x - i, y + i)
                && safe(x + i, y - i)
                && safe(x + i, y + i)
        })
    }
}
