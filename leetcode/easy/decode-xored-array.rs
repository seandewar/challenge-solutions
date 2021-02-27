// https://leetcode.com/problems/decode-xored-array
//
// Complexity: runtime O(n), space O(n).

impl Solution {
    pub fn decode(mut encoded: Vec<i32>, first: i32) -> Vec<i32> {
        encoded[0] ^= first;
        (1..encoded.len()).for_each(|i| encoded[i] ^= encoded[i - 1]);
        std::iter::once(first).chain(encoded).collect()
    }
}

// Alternative Solution: 2 lines, but unreadable.
//
// Might also give the compiler a bit more trouble in deciding whether or not bounds checking can
// be ignored here (which it also can).
//
// Complexity: runtime O(n), space O(n).

impl Solution {
    pub fn decode(mut encoded: Vec<i32>, first: i32) -> Vec<i32> {
        (0..encoded.len()).for_each(|i| encoded[i] ^= *encoded.get(i - 1).unwrap_or(&first));
        std::iter::once(first).chain(encoded).collect()
    }
}
