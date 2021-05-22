// https://leetcode.com/problems/n-queens
//
// Back-tracking + Recursion.
// This has room for optimization.
//
// Complexity: runtime O(n^3), space O(n^2).

impl Solution {
    pub fn solve_n_queens(n: i32) -> Vec<Vec<String>> {
        let (mut res, mut board) = (vec![], vec![vec![false; n as _]; n as _]);
        Self::dfs(&mut board, n as _, &mut res, 0);
        res
    }

    fn dfs(board: &mut Vec<Vec<bool>>, queens: usize, out: &mut Vec<Vec<String>>, from_y: usize) {
        if queens == 0 {
            out.push(
                board
                    .iter()
                    .map(|r| r.iter().map(|&s| if s { 'Q' } else { '.' }).collect())
                    .collect(),
            );
        } else {
            let n = board.len();
            for y in from_y..n {
                for x in 0..n {
                    if Self::safe_square(board, x, y) {
                        board[y][x] = true;
                        Self::dfs(board, queens - 1, out, y + 1); // can skip this row
                        board[y][x] = false; // back-track
                    }
                }
            }
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
