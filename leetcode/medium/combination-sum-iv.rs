// https://leetcode.com/problems/combination-sum-iv
//
// DFS + Memoization DP.
// Complexity: runtime O(n * target), space O(target).

impl Solution {
    pub fn combination_sum4(nums: Vec<i32>, target: i32) -> i32 {
        let mut mem = vec![-1; 1000];
        mem[0] = 1;
        Self::dfs(&nums[..], target, &mut mem[..])
    }

    fn dfs(nums: &[i32], target: i32, mem: &mut [i32]) -> i32 {
        let i = target as usize;
        if mem[i] == -1 {
            mem[i] = nums
                .iter()
                .filter(|&&x| target >= x)
                .map(|x| Self::dfs(nums, target - x, mem))
                .sum();
        }
        mem[i]
    }
}
