// https://leetcode.com/problems/push-dominoes
//
// Not the most efficient solution; in retrospect, this can be done much better by storing the new
// dominoes array as an array of ints, decrementing when a domino is pushed to the left,
// incrementing when pushed to the right.
//
// The int array can be updated by 2 linear iterations of the array; from left-to-right to update
// Rs, and the other in reverse right-to-left order to update Ls.
//
// Convert the int array into the String to return by
// converting negative ints to Ls and positives to Rs (0s to .s).
//
// Complexity: runtime O(n), space O(n).

use std::collections::VecDeque;

impl Solution {
    pub fn push_dominoes(dominoes: String) -> String {
        let mut dominoes = dominoes.into_bytes();
        let mut updates: VecDeque<_> = (0..dominoes.len())
            .filter(|&i| {
                let l_pushing = dominoes.get(i - 1).map_or(false, |&d| d == b'R');
                let r_pushing = dominoes.get(i + 1).map_or(false, |&d| d == b'L');
                dominoes[i] == b'.' && (l_pushing ^ r_pushing)
            })
            .collect();

        while !updates.is_empty() {
            let mut next_dominoes = dominoes.clone();
            for _ in 0..updates.len() {
                let i = updates.pop_front().unwrap();
                let l = dominoes.get(i - 1).copied().unwrap_or(b'.');
                let r = dominoes.get(i + 1).copied().unwrap_or(b'.');
                if r == b'L' && l != b'R' {
                    next_dominoes[i] = b'L';
                    if dominoes.get(i - 1).map_or(false, |&d| d == b'.') {
                        updates.push_back(i - 1);
                    }
                } else if l == b'R' && r != b'L' {
                    next_dominoes[i] = b'R';
                    if dominoes.get(i + 1).map_or(false, |&d| d == b'.') {
                        updates.push_back(i + 1);
                    }
                }
            }
            dominoes = next_dominoes;
        }

        String::from_utf8(dominoes).unwrap()
    }
}
