// https://leetcode.com/problems/valid-sodoku
// Lookup array solution.
// Complexity: runtime O(1), space O(1).

impl Solution {
    pub fn is_valid_sudoku(board: Vec<Vec<char>>) -> bool {
        let mark_seen = |seen: &mut [bool; 9], x: usize, y: usize| {
            let c = board[y][x];
            c != '.' && std::mem::replace(&mut seen[c as usize - '1' as usize], true)
        };

        for i in 0..3 {
            for j in 0..3 {
                let mut seen = [false; 9];
                for y in 0..3 {
                    for x in 0..3 {
                        if mark_seen(&mut seen, j * 3 + x, i * 3 + y) {
                            return false;
                        }
                    }
                }
            }
        }
        for i in 0..9 {
            let (mut seeny, mut seenx) = ([false; 9], [false; 9]);
            for j in 0..9 {
                if mark_seen(&mut seeny, j, i) || mark_seen(&mut seenx, i, j) {
                    return false;
                }
            }
        }
        true
    }
}

// Alternative Solution: Bit manipulation.
// Complexity: runtime O(1), space O(1).

impl Solution {
    pub fn is_valid_sudoku(board: Vec<Vec<char>>) -> bool {
        let mark_seen = |seen: &mut u16, x: usize, y: usize| match board[y][x] {
            '.' => false,
            c => {
                let mask = 1 << (c as usize - '1' as usize);
                let was_seen = *seen & mask != 0;
                *seen |= mask;
                was_seen
            }
        };

        for i in 0..3 {
            for j in 0..3 {
                let mut seen = 0;
                for y in 0..3 {
                    for x in 0..3 {
                        if mark_seen(&mut seen, j * 3 + x, i * 3 + y) {
                            return false;
                        }
                    }
                }
            }
        }
        for i in 0..9 {
            let (mut seeny, mut seenx) = (0, 0);
            for j in 0..9 {
                if mark_seen(&mut seeny, j, i) || mark_seen(&mut seenx, i, j) {
                    return false;
                }
            }
        }
        true
    }
}
