// https://leetcode.com/problems/max-area-of-island
//
// Iterative DFS.
// Complexity: runtime O(n*m), space O(n*m).

impl Solution {
    pub fn max_area_of_island(mut grid: Vec<Vec<i32>>) -> i32 {
        let (mut stack, mut max_area) = (vec![], 0);
        for y in 0..grid.len() {
            for x in 0..grid[y].len() {
                let mut area = 0;
                stack.push((x, y));
                while let Some((x, y)) = stack.pop() {
                    if std::mem::replace(&mut grid[y][x], 0) != 0 {
                        area += 1;
                        [(1, 0), (-1, 0), (0, 1), (0, -1)]
                            .iter()
                            .map(|(dx, dy)| {
                                ((x as isize + dx) as usize, (y as isize + dy) as usize)
                            })
                            .filter(|&(x, y)| y < grid.len() && x < grid[y].len())
                            .for_each(|xy| stack.push(xy));
                    }
                }
                max_area = max_area.max(area);
            }
        }
        max_area
    }
}
