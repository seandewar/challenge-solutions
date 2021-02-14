// https://leetcode.com/problems/shortest-path-in-binary-matrix
//
// BFS (Dijkstra's devolves into this when all edges are length 1).
// 
// This can be written much nicer with matches, but Leetcode only has Rust 1.40...
// This means we can't use matches!(), | in patterns, etc.
//
// Complexity: runtime O(w*h*log(w*h)), space O(w*h). [where w=width, h=height]

impl Solution {
    pub fn shortest_path_binary_matrix(mut grid: Vec<Vec<i32>>) -> i32 {
        let (w, h) = (grid[0].len() as i32, grid.len() as i32);
        let mut shortest_len = 1;
        let mut queue = std::collections::VecDeque::new();
        queue.push_back((0, 0));

        while !queue.is_empty() {
            let queue_len = queue.len();
            for _ in 0..queue_len {
                let (x, y) = queue.pop_front().unwrap();
                let v = grid
                    .get_mut(y as usize)
                    .map(|r| r.get_mut(x as usize))
                    .flatten();
                match (x, y, v) {
                    (_, _, None) => continue,
                    (_, _, Some(1)) => continue,
                    (x, y, _) if x == w - 1 && y == h - 1 => return shortest_len,
                    (x, y, Some(v)) => {
                        *v = 1; // mark as visited
                        for dy in -1..=1 {
                            for dx in -1..=1 {
                                if !(dx == 0 && dy == 0) {
                                    queue.push_back((x + dx, y + dy));
                                }
                            }
                        }
                    }
                }
            }
            shortest_len += 1;
        }

        -1
    }
}
