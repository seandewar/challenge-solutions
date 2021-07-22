// https://leetcode.com/problems/partition-array-into-disjoint-intervals
//
// Using (min) binary heaps.
// Can alternatively be achieved in O(n) time with some magic.
//
// Complexity: runtime O(n*logn), space O(n).

use std::{cmp::Reverse, collections::BinaryHeap};

impl Solution {
    pub fn partition_disjoint(nums: Vec<i32>) -> i32 {
        let mut right_heap: BinaryHeap<_> = nums.iter().map(|&x| Reverse(x)).collect();
        let (mut left_heap, mut left_max) = (BinaryHeap::new(), 0);
        nums.iter()
            .enumerate()
            .find(|(_, &x)| {
                left_heap.push(Reverse(x));
                while left_heap.peek() == right_heap.peek() {
                    left_heap.pop();
                    right_heap.pop();
                }
                left_max = left_max.max(x);
                left_max <= right_heap.peek().unwrap().0
            })
            .map(|(i, _)| i + 1)
            .unwrap() as _
    }
}
