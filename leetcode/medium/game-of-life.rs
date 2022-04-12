// https://leetcode.com/problems/game-of-life
//
// In-place solution.
//
// Works by using the 2nd LSB as the new value as to not overwrite the original when simulating the
// board. As such, requires a second pass of the board to shift the new value to the correct place
// afterwards.
//
// Complexity: runtime O(w*h), space O(1).

impl Solution {
    pub fn game_of_life(board: &mut Vec<Vec<i32>>) {
        let (w, h) = (board[0].len(), board.len());
        for x in 0..w {
            for y in 0..h {
                let neighbors = |x: usize, y: usize| {
                    let mut ret = 0;
                    for i in ((x as isize - 1).max(0) as usize)..(x + 2).min(w) {
                        for j in ((y as isize - 1).max(0) as usize)..(y + 2).min(h) {
                            if (i, j) != (x, y) && board[j][i] & 1 == 1 {
                                ret += 1;
                            }
                        }
                    }
                    ret
                };
                let new_val = match (board[y][x] & 1, neighbors(x, y)) {
                    (1, n) if n < 2 => 0,
                    (1, 2 | 3) => 1,
                    (1, n) if n > 3 => 0,
                    (0, 3) => 1,
                    (1, _) => 1,
                    _ => 0,
                };
                board[y][x] |= new_val << 1;
            }
        }

        for row in board {
            for val in row {
                *val >>= 1;
            }
        }
    }
}
