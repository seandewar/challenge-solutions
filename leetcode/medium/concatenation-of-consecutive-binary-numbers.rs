// https://leetcode.com/problems/concatenation-of-consecutive-binary-numbers
//
// Bitwise stuff.
// Complexity: runtime O(n), space O(1).

impl Solution {
    pub fn concatenated_binary(n: i32) -> i32 {
        let mut res = 0u64;
        for i in 1..=n as u64 {
            res <<= 64 - i.leading_zeros();
            res = (res | i) % 1_000_000_007;
        }
        res as i32
    }
}

// Alternative Solution: Using iterators and bitwise stuff.
// Complexity: runtime O(n), space O(1).

impl Solution {
    pub fn concatenated_binary(n: i32) -> i32 {
        (1..=n as u64).fold(0, |res, i| {
            ((res << 64 - i.leading_zeros()) | i) % 1_000_000_007
        }) as i32
    }
}

// Alternative Solution: Using iterators and powers of 2.
// Complexity: runtime O(n), space O(1).

impl Solution {
    pub fn concatenated_binary(n: i32) -> i32 {
        (1..=n as u64).fold(0, |res, i| {
            (res * 2u64.pow(64 - i.leading_zeros()) + i) % 1_000_000_007
        }) as i32
    }
}
