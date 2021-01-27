// https://leetcode.com/problems/path-with-minimum-effort
//
// Dijkstra's via min-heap.
// See code comments for details.
//
// Note that we store whether a cell was visited, not the minimum cost for each cell. This is due
// to Dijkstra's usage of a min-heap, which will always advance paths with the lowest total costs
// first, meaning that whenever a path visits a cell, that path will have the lowest cost possible
// to that cell.
//
// Complexity: runtime O(w*h*log(w*h)), space O(w*h). [where w=width, h=height]

use std::cmp::{max, Reverse};

impl Solution {
    pub fn minimum_effort_path(heights: Vec<Vec<i32>>) -> i32 {
        let (width, height) = (heights[0].len(), heights.len());
        let mut visited = vec![vec![false; width]; height];
        let mut path_heap = std::collections::BinaryHeap::new();
        path_heap.push(Reverse((0, 0, 0))); // start at (0, 0), with a 0 initial cost

        while let Some(Reverse((cost, x, y))) = path_heap.pop() {
            if y == height - 1 && x == width - 1 {
                return cost; // found the destination cell; we're finished!
            }
            if std::mem::replace(&mut visited[y][x], true) {
                continue; // cell already visited via a different path
            }

            // continue path in all possible valid directions from this cell
            for (dx, dy) in &[(1, 0), (-1, 0), (0, 1), (0, -1)] {
                let (new_x, new_y) = (x + *dx as usize, y + *dy as usize);
                // bounds check; note that a negative x/y will wrap around to a value likely >=
                // width/height of the board, which will also be caught by the check
                if new_x < width && new_y < height {
                    path_heap.push(Reverse((
                        max(cost, (heights[y][x] - heights[new_y][new_x]).abs()),
                        new_x,
                        new_y,
                    )));
                }
            }
        }

        unreachable!("all test inputs guarantee a path exists to the destination cell");
    }
}
