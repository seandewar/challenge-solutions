// https://leetcode.com/problems/replace-all-digits-with-characters
//
// Complexity: runtime O(n), space O(1).

impl Solution {
    pub fn replace_digits(mut s: String) -> String {
        unsafe { s.as_bytes_mut() }
            .chunks_exact_mut(2)
            .for_each(|c| c[1] = c[0] + c[1] - b'0');
        s
    }
}
