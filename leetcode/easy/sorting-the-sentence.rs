// https://leetcode.com/problems/sorting-the-sentence
//
// Complexity: runtime O(w*n*logn), space O(w*n) [where w == average word length in s, n == amount
//                                                of words in s].

impl Solution {
    pub fn sort_sentence(s: String) -> String {
        let mut s = s.split(" ").collect::<Vec<_>>();
        s.sort_unstable_by_key(|a| a.as_bytes()[a.len() - 1] - b'0');
        s.iter()
            .map(|w| unsafe { String::from_utf8_unchecked(w[..w.len() - 1].into()) })
            .collect::<Vec<_>>()
            .join(" ")
    }
}
