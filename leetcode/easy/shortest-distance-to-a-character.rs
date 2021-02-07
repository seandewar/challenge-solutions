// https://leetcode.com/problems/shortest-distance-to-a-character
//
// Complexity: runtime O(n), space O(n).

impl Solution {
    pub fn shortest_to_char(s: String, c: char) -> Vec<i32> {
        let c_idxs: Vec<_> = s
            .chars()
            .enumerate()
            .filter_map(|(i, x)| Some(i as i32).filter(|_| x == c))
            .collect();

        let mut near_idx = 0;
        (0..s.len() as i32)
            .map(|i| {
                let curr_dist = (c_idxs[near_idx] - i).abs();
                let next_dist = (c_idxs.get(near_idx + 1).unwrap_or(&std::i32::MAX) - i).abs();
                if next_dist < curr_dist {
                    near_idx += 1;
                    return next_dist;
                }
                curr_dist
            })
            .collect()
    }
}
