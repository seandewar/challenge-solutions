// https://leetcode.com/problems/number-of-1-bits
//
// Complexity: runtime O(1), space O(1).

impl Solution {
    pub fn hammingWeight(mut n: u32) -> i32 {
        let mut res = 0;
        while n != 0 {
            res += (n & 1) as i32;
            n >>= 1;
        }
        res
    }
}

// Alternative Solution: Iterator-based Solution with fold().
// Complexity: runtime O(1), space O(1).

impl Solution {
    pub fn hammingWeight(mut n: u32) -> i32 {
        (0..32).fold(0, |res, _| {
            let bit = n & 1;
            n >>= 1;
            res + bit as i32
        })
    }
}

// Alternative Solution: Using count_ones().
// Complexity: runtime O(1), space O(1).

impl Solution {
    pub fn hammingWeight(n: u32) -> i32 {
        n.count_ones() as _
    }
}

// Alternative Solution: Recursive Solution.
// Complexity: runtime O(1), space O(1).

impl Solution {
    pub fn hammingWeight(n: u32) -> i32 {
        match n {
            0 => 0,
            _ => (n & 1) as i32 + Self::hammingWeight(n >> 1),
        }
    }
}

// Alternative Solution: Powers of 2 Meme Solution.
// Complexity: runtime O(1), space O(1).

impl Solution {
    pub fn hammingWeight(mut n: u32) -> i32 {
        let mut res = 0;
        while n != 0 {
            res += (n % 2) as i32;
            n /= 2;
        }
        res
    }
}
