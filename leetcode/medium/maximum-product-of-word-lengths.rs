// https://leetcode.com/problems/maximum-product-of-word-lengths
//
// Using HashSet. May also be done with [_; 26].
//
// Would like to use flat_map().max() rather than map(max()).max(), but it requires a move into the
// inner closure, which isn't nice as we'll also end up moving the shadowing "words" variable.
//
// Complexity: runtime O(n^2), space O(n).

impl Solution {
    pub fn max_product(words: Vec<String>) -> i32 {
        let words: Vec<_> = words
            .into_iter()
            .map(|w| (w.chars().collect::<std::collections::HashSet<_>>(), w))
            .collect();

        words
            .iter()
            .enumerate()
            .map(|(i, (cset, w))| {
                words[i + 1..]
                    .iter()
                    .filter(|(cset2, _)| cset.intersection(cset2).next().is_none())
                    .map(|(_, w2)| w.len() * w2.len())
                    .max()
                    .unwrap_or(0)
            })
            .max()
            .unwrap_or(0) as _
    }
}
