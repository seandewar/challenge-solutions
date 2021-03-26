// https://leetcode.com/problems/word-subsets
//
// Complexity: runtime O(a+b), space O(a) [O(1) ignoring output].

impl Solution {
    pub fn word_subsets(a: Vec<String>, b: Vec<String>) -> Vec<String> {
        let count = |s: &str| {
            let mut m = [0; 26];
            s.chars().for_each(|c| m[c as usize - 'a' as usize] += 1);
            m
        };
        let b_counts = b.iter().map(|s| count(s)).fold([0; 26], |mut bm, m| {
            m.iter().enumerate().for_each(|(i, &n)| bm[i] = bm[i].max(n));
            bm
        });

        a.into_iter()
            .map(|w| (count(&w), w))
            .filter(|(m, _)| b_counts.iter().enumerate().all(|(i, &n)| m[i] >= n))
            .map(|(_, w)| w)
            .collect()
    }
}
