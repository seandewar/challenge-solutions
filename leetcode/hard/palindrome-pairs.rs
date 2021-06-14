// https://leetcode.com/problems/palindrome-pairs
//
// Complexity: runtime O(n*w^2), space O(n) [excluding output]. [where n == words.len(), w == word
//                                                               size]

use std::collections::HashMap;

impl Solution {
    pub fn palindrome_pairs(words: Vec<String>) -> Vec<Vec<i32>> {
        let word_map: HashMap<_, _> = words
            .iter()
            .enumerate()
            .map(|(i, w)| (w.as_str(), i))
            .collect();

        words.iter().enumerate().fold(vec![], |mut acc, (i, w)| {
            let n = w.len();
            let rev: String = w.chars().rev().collect();
            let from_right = (0..n)
                .filter(|&k| w[..n - k].chars().eq(w[..n - k].chars().rev()))
                .filter_map(|k| word_map.get(&rev[..k]).map(|&j| (j, i)));

            (0..=n)
                .filter(|&k| w[k..].chars().eq(w[k..].chars().rev()))
                .filter_map(|k| word_map.get(&rev[n - k..]).map(|&j| (i, j)))
                .chain(from_right)
                .filter(|&(i, j)| i != j)
                .for_each(|(i, j)| acc.push(vec![i as _, j as _]));

            acc
        })
    }
}
