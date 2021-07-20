// https://leetcode.com/problems/shuffle-an-array
//
// Using shuffle() from the rand library.
// Complexity: runtime O(n), space O(n).

use rand::seq::SliceRandom;

struct Solution {
    nums: Vec<i32>,
}

impl Solution {
    fn new(nums: Vec<i32>) -> Self {
        Self { nums }
    }

    /** Resets the array to its original configuration and return it. */
    fn reset(&self) -> Vec<i32> {
        self.nums.clone()
    }

    /** Returns a random shuffling of the array. */
    fn shuffle(&self) -> Vec<i32> {
        let mut shuffled = self.nums.clone();
        shuffled.shuffle(&mut rand::thread_rng());
        shuffled
    }
}

// Alternative Solution: Shuffling manually using the rand library.
// See C++ solution for details.
// Complexity: runtime O(n), space O(n).

use rand::Rng;

struct Solution {
    nums: Vec<i32>,
}

impl Solution {
    fn new(nums: Vec<i32>) -> Self {
        Self { nums }
    }

    /** Resets the array to its original configuration and return it. */
    fn reset(&self) -> Vec<i32> {
        self.nums.clone()
    }

    /** Returns a random shuffling of the array. */
    fn shuffle(&self) -> Vec<i32> {
        let mut shuffled = self.nums.clone();
        let n = shuffled.len();
        for i in 0..n {
            shuffled.swap(i, rand::thread_rng().gen_range(i, n));
        }
        shuffled
    }
}
