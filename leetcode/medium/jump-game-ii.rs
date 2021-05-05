// https://leetcode.com/problems/jump-game-ii
//
// Constant space.
// Complexity: runtime O(n), space O(1).

impl Solution {
    pub fn jump(nums: Vec<i32>) -> i32 {
        nums.iter()
            .take(nums.len() - 1)
            .enumerate()
            .fold((0, 0, 0), |(mut maxi, prev_maxi, res), (i, &x)| {
                maxi = maxi.max(i + x as usize);
                if i == prev_maxi {
                    (maxi, maxi, res + 1)
                } else {
                    (maxi, prev_maxi, res)
                }
            })
            .2
    }
}

// Alternative Solution: Using a memoization array.
// Complexity: runtime O(n), space O(n).

impl Solution {
    pub fn jump(nums: Vec<i32>) -> i32 {
        let mut dp = vec![std::i32::MAX; nums.len()];
        dp[0] = 0;
        nums.into_iter().enumerate().for_each(|(i, x)| {
            let end = (i + x as usize).min(dp.len() - 1);
            (i + 1..=end).for_each(|j| dp[j] = dp[j].min(dp[i] + 1));
        });

        dp[dp.len() - 1]
    }
}
