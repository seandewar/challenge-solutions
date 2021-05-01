// https://leetcode.com/problems/prefix-and-suffix-search
//
// Using HashMaps and BTreeSets.
//
// We could *technically* use slices for the map keys, but it's easier to just use Strings than to
// fight satisfy the borrow checker or use unsafe code.
//
// We also don't really need the BTreeSets (a sorted Vec is enough), but they naturally implement
// an intersection() operation for us, which is nice.
//
// Complexity: too lazy; not too complicated, though. :)

use std::{
    cmp::Reverse,
    collections::{BTreeSet, HashMap},
};

struct WordFilter {
    pre_map: HashMap<String, BTreeSet<Reverse<usize>>>,
    suf_map: HashMap<String, BTreeSet<Reverse<usize>>>,
}

impl WordFilter {
    fn new(words: Vec<String>) -> Self {
        let (mut pre_map, mut suf_map) = (HashMap::new(), HashMap::new());
        for (i, w) in words.iter().enumerate() {
            for len in 1..=w.len() {
                pre_map
                    .entry(w[..len].to_owned())
                    .or_insert(BTreeSet::new())
                    .insert(Reverse(i));
                suf_map
                    .entry(w[w.len() - len..].to_owned())
                    .or_insert(BTreeSet::new())
                    .insert(Reverse(i));
            }
        }

        Self { pre_map, suf_map }
    }

    fn f(&self, prefix: String, suffix: String) -> i32 {
        let (pres, sufs) = (self.pre_map.get(&prefix), self.suf_map.get(&suffix));
        if let (Some(pres), Some(sufs)) = (pres, sufs) {
            pres.intersection(sufs)
                .map(|i| i.0 as _)
                .next()
                .unwrap_or(-1)
        } else {
            -1
        }
    }
}
