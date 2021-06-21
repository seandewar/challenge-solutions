// https://leetcode.com/problems/swim-in-rising-water
//
// BFS Solution.
// Complexity: runtime O(n^2), space O(n^2).

use std::collections::VecDeque;

impl Solution {
    pub fn swim_in_water(mut grid: Vec<Vec<i32>>) -> i32 {
        let (mut q, mut q2, n) = (VecDeque::new(), VecDeque::new(), grid.len());
        let mut t = std::mem::replace(&mut grid[0][0], -1);
        q.push_back((0, 0, t));
        loop {
            while let Some((x, y, t2)) = q.pop_front() {
                if t2 > t {
                    q2.push_back((x, y, t2));
                    continue;
                }
                if (x, y) == (n - 1, n - 1) {
                    return t;
                }
                [(1, 0), (-1, 0), (0, 1), (0, -1)]
                    .iter()
                    .map(|(dx, dy)| ((x as isize + dx) as usize, (y as isize + dy) as usize))
                    .filter(|&(x, y)| x < n && y < n)
                    .for_each(|(x, y)| {
                        if grid[y][x] >= 0 {
                            q.push_back((x, y, std::mem::replace(&mut grid[y][x], -1)))
                        }
                    });
            }
            q.append(&mut q2);
            t += 1;
        }
        unreachable!()
    }
}
