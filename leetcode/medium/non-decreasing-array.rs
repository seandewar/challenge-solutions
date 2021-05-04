// https://leetcode.com/problems/non-decreasing-array
//
// Complexity: runtime O(n), space O(1).

impl Solution {
    pub fn check_possibility(nums: Vec<i32>) -> bool {
        nums.windows(2).filter(|w| w[0] > w[1]).count() < 2
            && nums
                .windows(4)
                .all(|w| w[2] >= w[1] || w[3] >= w[1] || w[2] >= w[0])
    }
}

// Alternative Solution: My initial attempt. Ain't gonna explain it :)
// Complexity: runtime O(n), space O(1).

impl Solution {
    pub fn check_possibility(nums: Vec<i32>) -> bool {
        let (n, non_dec) = (nums.len(), |i| nums[i - 1] <= nums[i]);
        (1..n).find(|&i| !non_dec(i)).map_or(true, |i| {
            let modify1 = (i == 1 || nums[i - 2] <= nums[i]) && (i + 1..n).all(non_dec);
            let modify2 = (i == n - 1 || nums[i - 1] <= nums[i + 1]) && (i + 2..n).all(non_dec);
            modify1 || modify2
        })
    }
}
