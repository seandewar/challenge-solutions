// https://leetcode.com/problems/count-the-number-of-fair-pairs
// Faster than 100%.
// Complexity: runtime O(nlogn), space O(logn).

impl Solution {
    pub fn count_fair_pairs(mut nums: Vec<i32>, lower: i32, upper: i32) -> i64 {
        nums.sort_unstable();

        nums.iter()
            .enumerate()
            .map(|(i, &x)| {
                let l = nums[i + 1..].partition_point(|&y| y < lower - x) + i + 1;
                let r = nums[l..].partition_point(|&y| y <= upper - x) + l;
                i64::try_from(r - l).unwrap()
            })
            .sum()
    }
}
