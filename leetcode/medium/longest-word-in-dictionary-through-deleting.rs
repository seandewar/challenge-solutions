// https://leetcode.com/problems/longest-word-in-dictionary-through-deleting
//
// Complexity: runtime O(mn + mn*logn), space O(logn) [due to sort_unstable_by() using
// (pattern-defeating) quicksort]. [where n == d.len(), m == average String len() in d]

impl Solution {
    pub fn find_longest_word(s: String, mut d: Vec<String>) -> String {
        d.sort_unstable_by(|a, b| b.len().cmp(&a.len()).then_with(|| a.cmp(b)));

        for s2 in d {
            let mut s2_iter = s2.chars().peekable();
            for c in s.chars() {
                if let Some(&c2) = s2_iter.peek() {
                    if c == c2 {
                        s2_iter.next();
                    }
                } else {
                    break;
                }
            }

            if s2_iter.peek().is_none() {
                return s2;
            }
        }

        String::new()
    }
}
