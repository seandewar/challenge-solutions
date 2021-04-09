// https://leetcode.com/problems/verifying-an-alien-dictionary
//
// These solutions abuse the Rust standard library's absence of is_sorted_by(), instead using
// sort_unstable_by() to solve this problem by comparing our sorted words with the input words
// (there shouldn't be a difference if input words were already sorted!)
//
// See the C++ solution for this problem for a more standard approach.
//
// Complexity: runtime O(n*logn), space O(logn).

use std::cmp::Ordering;

impl Solution {
    pub fn is_alien_sorted(words: Vec<String>, order: String) -> bool {
        let mut words_sorted = words.clone();
        words_sorted.sort_unstable_by(|a, b| {
            a.chars()
                .zip(b.chars())
                .try_fold(a.len().cmp(&b.len()), |len_ord, (a, b)| {
                    if a == b {
                        Ok(len_ord)
                    } else {
                        Err(if order.chars().find(|&x| x == a || x == b).unwrap() == a {
                            Ordering::Less
                        } else {
                            Ordering::Greater
                        })
                    }
                })
                .unwrap_or_else(|ord| ord)
        });

        words_sorted == words
    }
}
