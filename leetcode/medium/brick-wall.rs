// https://leetcode.com/problems/brick-wall
//
// Complexity: runtime O(total bricks), space O(max bricks in a row).

impl Solution {
    pub fn least_bricks(wall: Vec<Vec<i32>>) -> i32 {
        let mut counts = std::collections::HashMap::new();
        for row in &wall {
            let mut x = 0;
            for w in &row[..row.len() - 1] {
                x += w;
                *counts.entry(x).or_insert(0) += 1;
            }
        }
        (wall.len() - counts.values().max().unwrap_or(&0)) as _
    }
}
