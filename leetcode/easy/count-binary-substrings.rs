// https://leetcode.com/problems/count-binary-substrings
//
// Complexity: runtime O(n), space O(1).

impl Solution {
    pub fn count_binary_substrings(s: String) -> i32 {
        let s = s.into_bytes();
        let (mut res, mut i, mut prev_len) = (0, 0, 0);
        while i < s.len() {
            let j = (i + 1..s.len()).find(|&j| s[i] != s[j]).unwrap_or(s.len());
            res += prev_len.min(j - i);
            prev_len = j - i;
            i = j;
        }
        res as _
    }
}
