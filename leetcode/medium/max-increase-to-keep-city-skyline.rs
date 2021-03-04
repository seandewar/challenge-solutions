// https://leetcode.com/problems/max-increase-to-keep-city-skyline
//
// Complexity: runtime O(m*n), space O(m+n).

impl Solution {
    pub fn max_increase_keeping_skyline(grid: Vec<Vec<i32>>) -> i32 {
        let (m, n) = (grid.len(), grid[0].len());
        let row_maxs: Vec<_> = grid.iter().map(|row| row.iter().max().unwrap()).collect();
        let col_maxs: Vec<_> = (0..n)
            .map(|x| (0..m).map(|y| grid[y][x]).max().unwrap())
            .collect();

        (0..n)
            .map(|x| {
                (0..m)
                    .map(|y| row_maxs[y].min(&col_maxs[x]) - grid[y][x])
                    .sum::<i32>()
            })
            .sum()
    }
}
