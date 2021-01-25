// https://leetcode.com/problems/check-if-all-1s-are-at-least-length-k-places-away
//
// Iterator-based solution.
// Complexity: runtime O(n), space O(1).

impl Solution {
    pub fn k_length_apart(nums: Vec<i32>, k: i32) -> bool {
        nums.into_iter()
            .try_fold(0, |rem, x| match x {
                1 if rem > 0 => None,
                1 => Some(k),
                _ => Some(rem - 1),
            })
            .is_some()
    }
}

// Alternative Solution: Imperative solution.
// Complexity: runtime O(n), space O(1).

impl Solution {
    pub fn k_length_apart(nums: Vec<i32>, k: i32) -> bool {
        let mut rem = 0;
        for x in nums {
            if x == 1 {
                if rem > 0 {
                    return false;
                }
                rem = k;
            } else {
                rem -= 1;
            }
        }

        true
    }
}
