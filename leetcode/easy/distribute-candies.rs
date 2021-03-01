// https://leetcode.com/problems/distribute-candies
//
// Complexity: runtime O(n), space O(n).

impl Solution {
    pub fn distribute_candies(candy_type: Vec<i32>) -> i32 {
        candy_type
            .iter()
            .collect::<std::collections::HashSet<_>>()
            .len()
            .min(candy_type.len() / 2) as _
    }
}

// Alternative Solution: Sort & dedup() (similiar to std::unique() in C++).
// Complexity: runtime O(n*logn), space O(logn).

impl Solution {
    pub fn distribute_candies(mut candy_type: Vec<i32>) -> i32 {
        let n = candy_type.len();
        candy_type.sort_unstable();
        candy_type.dedup();
        candy_type.len().min(n / 2) as _
    }
}
