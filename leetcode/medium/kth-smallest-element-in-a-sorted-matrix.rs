// https://leetcode.com/problems/kth-smallest-element-in-a-sorted-matrix
//
// Pushing everything onto the heap.
// Complexity: runtime O(m*n*logn), space O(m*n).

use std::{cmp::Reverse, collections::BinaryHeap};

impl Solution {
    pub fn kth_smallest(matrix: Vec<Vec<i32>>, k: i32) -> i32 {
        let mut heap: BinaryHeap<_> = matrix
            .iter()
            .flat_map(|r| r.iter().map(|&x| Reverse(x)))
            .collect();
        for _ in 1..k {
            heap.pop();
        }
        heap.peek().unwrap().0
    }
}

// Alternative Solution: Pushing into the heap on-demand.
// Complexity: runtime O(m*n*logm), space O(m).

use std::{cmp::Reverse, collections::BinaryHeap};

impl Solution {
    pub fn kth_smallest(matrix: Vec<Vec<i32>>, mut k: i32) -> i32 {
        let (m, n) = (matrix.len(), matrix[0].len());
        let mut heap: BinaryHeap<_> = matrix
            .iter()
            .enumerate()
            .map(|(i, r)| Reverse((r[0], i, 0)))
            .collect();

        while let Some(Reverse((v, r, c))) = heap.pop() {
            k -= 1;
            if k == 0 {
                return v;
            }
            if c + 1 < n {
                heap.push(Reverse((matrix[r][c + 1], r, c + 1)));
            }
        }

        unreachable!()
    }
}
