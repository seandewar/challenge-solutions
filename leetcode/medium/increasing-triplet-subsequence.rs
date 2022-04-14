// https://leetcode.com/problems/increasing-triplet-subsequence
//
// Complexity: runtime O(n), space O(1).

impl Solution {
    pub fn increasing_triplet(nums: Vec<i32>) -> bool {
        let (mut x, mut y) = (nums[0], i32::MAX);
        for &v in &nums[1..] {
            if v <= x {
                x = v;
            } else if v <= y {
                y = v;
            } else {
                return true;
            }
        }

        false
    }
}

// Alternative Solution: BTree abuse.
// Complexity: runtime O(n^2), space O(n).

use std::{collections::BTreeMap, ops::Bound::*};

impl Solution {
    pub fn increasing_triplet(nums: Vec<i32>) -> bool {
        let mut map = BTreeMap::new();
        for (i, &v) in nums.iter().enumerate() {
            map.entry(v)
                .and_modify(|t: &mut (usize, usize)| *t = (t.0, i.max(t.1)))
                .or_insert((i, i));
        }

        for j in 1..(nums.len() - 1) {
            let y = nums[j];

            if let Some(_) = map.range(..y).find(|&(_, &(i, _))| i < j) {
                if let Some(_) = map
                    .range((Excluded(y), Unbounded))
                    .find(|&(_, &(_, k))| j < k)
                {
                    return true;
                }
            }
        }

        false
    }
}
