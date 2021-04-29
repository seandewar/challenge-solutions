// https://leetcode.com/problems/max-consecutive-ones
//
// Complexity: runtime O(n), space O(1).

impl Solution {
    pub fn find_max_consecutive_ones(nums: Vec<i32>) -> i32 {
        nums.iter()
            .scan(0, |streak, &x| {
                *streak += 1;
                if x != 1 {
                    *streak = 0;
                }
                Some(*streak)
            })
            .max()
            .unwrap()
    }
}

// Alternative Solution: Using split() and splitting at each 0.
// Complexity: runtime O(n), space O(1).

impl Solution {
    pub fn find_max_consecutive_ones(nums: Vec<i32>) -> i32 {
        nums.split(|&n| n == 0)
            .map(|slice| slice.len())
            .max()
            .unwrap() as _
    }
}
