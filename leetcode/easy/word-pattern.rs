// https://leetcode.com/problems/word-pattern
// Complexity: runtime O(p+s), space O(p+s).

use std::collections::{
    hash_map::Entry::{Occupied, Vacant},
    HashMap, HashSet,
};

impl Solution {
    pub fn word_pattern(pattern: String, s: String) -> bool {
        let (mut p_iter, mut s_iter) = (pattern.chars(), s.split_ascii_whitespace());
        let (mut map, mut set) = (HashMap::new(), HashSet::new());

        while let tuple = (p_iter.next(), s_iter.next()) {
            match tuple {
                (None, None) => break,
                (Some(p), Some(word)) => match map.entry(p) {
                    Vacant(v) if set.insert(word) => {
                        v.insert(word);
                    }
                    Occupied(o) if word == *o.get() => (),
                    _ => return false,
                },
                _ => return false,
            }
        }

        true
    }
}
