// https://leetcode.com/problems/keys-and-rooms
//
// Complexity: runtime O(k), space O(k). [where k == total number of keys in all rooms]

use std::{collections::HashSet, iter::once};

impl Solution {
    pub fn can_visit_all_rooms(rooms: Vec<Vec<i32>>) -> bool {
        let mut visited: HashSet<_> = once(0).collect();
        let mut to_open = rooms[0].clone();
        while let Some(key) = to_open.pop() {
            if visited.insert(key) {
                to_open.extend(&rooms[key as usize]);
            }
        }
        visited.len() == rooms.len()
    }
}
