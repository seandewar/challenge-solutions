// https://leetcode.com/problems/top-k-frequent-elements/
//
// Using a hash map and sort.
// Complexity: runtime O(n*logn) [O(n+k*logn) possible with a partial sort], space O(n).

use std::collections::HashMap;

impl Solution {
    pub fn top_k_frequent(nums: Vec<i32>, k: i32) -> Vec<i32> {
        let mut counts = HashMap::new();
        for x in nums {
            *counts.entry(x).or_insert(0) += 1;
        }

        // a partial sort would be nicer, but it doesn't exist in the standard library
        let mut nums_by_freq = counts.keys().copied().collect::<Vec<_>>();
        nums_by_freq.sort_unstable_by_key(|x| counts[x]);
        nums_by_freq.reverse();

        nums_by_freq[..k as usize].to_vec()
    }
}

// Alternative Solution: using max heap and hash map.
// Complexity: runtime likely O(n*logn) [O(n+k*logn) if using advice described within code comments
// below], space O(n).

use std::cmp::Ordering;
use std::collections::{BinaryHeap, HashMap};

#[derive(Eq)]
struct HeapCount {
    num: i32,
    count: i32,
}

impl PartialEq for HeapCount {
    fn eq(&self, other: &Self) -> bool {
        self.count == other.count
    }
}

impl Ord for HeapCount {
    fn cmp(&self, other: &Self) -> Ordering {
        self.count.cmp(&other.count)
    }
}

impl PartialOrd for HeapCount {
    fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
        Some(self.cmp(other))
    }
}

impl Solution {
    pub fn top_k_frequent(nums: Vec<i32>, k: i32) -> Vec<i32> {
        let mut counts = HashMap::new();
        for x in nums {
            *counts.entry(x).or_insert(0) += 1;
        }

        let mut max_heap = BinaryHeap::with_capacity(counts.len());
        for (k, v) in counts {
            max_heap.push(HeapCount { num: k, count: v });
        }

        // into_iter_sorted() in combination with std::cmp::Reverse() or a reversed Ord will be
        // better here (to create a min-heap), but it's currently unstable. the more performant
        // currently-available solution would be to imperatively loop k times and manually pop() to
        // construct the Vec, while using a reversed heap ordering like described previously
        max_heap
            .into_sorted_vec()
            .iter()
            .rev()
            .take(k as usize)
            .map(|x| x.num)
            .collect()
    }
}
