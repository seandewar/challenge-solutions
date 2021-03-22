// https://leetcode.com/problems/find-the-highest-altitude
//
// Complexity: runtime O(n), space O(1).

impl Solution {
    pub fn largest_altitude(gain: Vec<i32>) -> i32 {
        gain.into_iter()
            .fold((0, 0), |(res, h), g| (res.max(h + g), h + g))
            .0
    }
}

// Alternative Solution: scan()
// Complexity: runtime O(n), space O(1).

impl Solution {
    pub fn largest_altitude(gain: Vec<i32>) -> i32 {
        std::iter::once(0)
            .chain(gain.into_iter())
            .scan(0, |h, g| {
                *h += g;
                Some(*h)
            })
            .max()
            .unwrap()
    }
}
