// https://leetcode.com/problems/open-the-lock
//
// Complexity: technically, constant due to the constraints; runtime O(1), space O(1). (I'm lazy)

use std::collections::{HashSet, VecDeque};

impl Solution {
    pub fn open_lock(deadends: Vec<String>, target: String) -> i32 {
        let mut ignore: HashSet<_> = deadends.iter().map(|s| s.parse().unwrap()).collect();
        let (target, mut queue, mut moves) = (target.parse::<u16>().unwrap(), VecDeque::new(), 0);
        queue.push_back(0000);
        while !queue.is_empty() {
            for _ in 0..queue.len() {
                let lock = queue.pop_front().unwrap();
                if lock == target {
                    return moves;
                } else if ignore.insert(lock) {
                    for i in &[1000, 0100, 0010, 0001] {
                        let wheel = (lock / i) % 10;
                        queue.push_back((lock - i * wheel) + (i * ((wheel + 1) % 10)));
                        queue.push_back((lock - i * wheel) + (i * ((wheel + 9) % 10)));
                    }
                }
            }
            moves += 1;
        }
        -1
    }
}
