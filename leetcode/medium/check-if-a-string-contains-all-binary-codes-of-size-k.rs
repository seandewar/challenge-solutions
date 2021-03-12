// https://leetcode.com/problems/check-if-a-string-contains-all-binary-codes-of-size-k
//
// Complexity: runtime O(n+2^k), space O(2^k).

impl Solution {
    pub fn has_all_codes(s: String, k: i32) -> bool {
        if k as usize > s.len() {
            return false;
        }

        let mut sub = s
            .chars()
            .take(k as _)
            .fold(0, |acc, d| (acc << 1) | d.to_digit(2).unwrap());

        let substrs: std::collections::HashSet<_> = std::iter::once(sub)
            .chain(s.chars().skip(k as _).map(|d| {
                sub = (sub << 1) | d.to_digit(2).unwrap();
                sub &= (1 << k) - 1;
                sub
            }))
            .collect();

        (0..1 << k).all(|x| substrs.contains(&x))
    }
}
