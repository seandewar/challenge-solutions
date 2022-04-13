// https://leetcode.com/problems/spiral-matrix-ii
//
// Complexity: runtime O(n^2), space O(1) [O(n^2) counting output].

impl Solution {
    pub fn generate_matrix(n: i32) -> Vec<Vec<i32>> {
        let mut mat = vec![vec![0; n as usize]; n as usize];

        let (mut x, mut y): (i32, i32) = (0, 0);
        let dirs = [(1, 0), (0, 1), (-1, 0), (0, -1)];
        let mut dir_i = 0;
        let mut step = 1;
        loop {
            mat[y as usize][x as usize] = step;

            let old_dir_i = dir_i;
            loop {
                let (x2, y2) = (x + dirs[dir_i].0, y + dirs[dir_i].1);
                if x2 >= 0 && y2 >= 0 && x2 < n && y2 < n && mat[y2 as usize][x2 as usize] == 0 {
                    x = x2; // destructed assignments are still experimental (cringe)
                    y = y2;
                    break;
                }

                dir_i = (dir_i + 1) % dirs.len();
                if dir_i == old_dir_i {
                    return mat;
                }
            }

            step += 1;
        }
    }
}
