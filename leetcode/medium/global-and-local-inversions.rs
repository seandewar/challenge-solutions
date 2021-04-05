// https://leetcode.com/problems/global-and-local-inversions
//
// Complexity: runtime O(n^2), space O(n).

use std::collections::{btree_map::Entry::Occupied, BTreeMap};

impl Solution {
    pub fn is_ideal_permutation(a: Vec<i32>) -> bool {
        let local = (1..a.len()).filter(|&i| a[i] < a[i - 1]).count();
        let mut bst = BTreeMap::new();
        a.iter().for_each(|&x| *bst.entry(x).or_insert(0) += 1);

        a.iter()
            .try_fold(0, |acc, &x| {
                if let Occupied(mut o) = bst.entry(x) {
                    *o.get_mut() -= 1;
                    if *o.get() == 0 {
                        o.remove();
                    }
                } else {
                    unreachable!();
                }

                Some(acc + bst.range(..x).map(|(_, n)| n).sum::<usize>())
                    .filter(|&acc| acc <= local)
            })
            .is_some()
    }
}

// Alternative Solution: Linear-time one-liner.
// Complexity: runtime O(n), space O(1).

impl Solution {
    pub fn is_ideal_permutation(a: Vec<i32>) -> bool {
        a.iter()
            .enumerate()
            .all(|(i, &x)| (x - i as i32).abs() <= 1)
    }
}
