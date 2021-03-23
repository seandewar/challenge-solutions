// https://leetcode.com/problems/3sum-with-multiplicity
//
// Condensed for line count :)
// Complexity: runtime O(n^2 * logn), space O(n).

use std::collections::HashMap;

impl Solution {
    pub fn three_sum_multi(arr: Vec<i32>, target: i32) -> i32 {
        let (mut kmap, mut res) = (HashMap::new(), 0);
        (0..arr.len()).for_each(|i| kmap.entry(arr[i]).or_insert(vec![]).push(i));

        for i in 0..arr.len() {
            for j in i + 1..arr.len() {
                if let Some(ks) = kmap.get(&(target - arr[i] - arr[j])) {
                    res += ks.len() - ks.binary_search(&(j + 1)).unwrap_or_else(|ki| ki);
                }
            }
        }

        (res % 1_000_000_007) as _
    }
}

// Alternative Solution: Declarative and reduced for line count.
// Complexity: runtime O(n^2 * logn), space O(n).

use std::collections::HashMap;

impl Solution {
    pub fn three_sum_multi(arr: Vec<i32>, target: i32) -> i32 {
        let mut kmap = HashMap::new();
        (0..arr.len()).for_each(|i| kmap.entry(arr[i]).or_insert(vec![]).push(i));

        ((0..arr.len())
            .flat_map(|i| {
                (i + 1..arr.len())
                    .map(move |j| (i, j))
                    .filter_map(|(i, j)| {
                        kmap.get(&(target - arr[i] - arr[j])).map(|ks| {
                            ks.len() - ks.binary_search(&(j + 1)).unwrap_or_else(|ki| ki)
                        })
                    })
            })
            .sum::<usize>()
            % 1_000_000_007) as _
    }
}
