// https://leetcode.com/problems/sliding-window-maximum
//
// Using Max Heap & Hash Map.
//
// Can potentially be done in O(n) with a slightly different approach involving a VecDeque.
//
// Complexity: runtime O(n*logn), space O(n).

use std::collections::{hash_map::Entry::Occupied, BinaryHeap, HashMap};

impl Solution {
    pub fn max_sliding_window(nums: Vec<i32>, k: i32) -> Vec<i32> {
        let k = k as usize;
        let mut old_counts = HashMap::new();
        let mut heap: BinaryHeap<_> = nums[..k].iter().copied().collect();
        let mut res = Vec::with_capacity(nums.len() - k + 1);
        res.push(*heap.peek().unwrap());

        for w in nums.windows(k + 1) {
            heap.push(w[w.len() - 1]);
            *old_counts.entry(w[0]).or_insert(0) += 1;

            while let Occupied(mut o) = old_counts.entry(*heap.peek().unwrap()) {
                heap.pop();
                *o.get_mut() -= 1;
                if *o.get() == 0 {
                    o.remove();
                }
            }

            res.push(*heap.peek().unwrap());
        }

        res
    }
}
