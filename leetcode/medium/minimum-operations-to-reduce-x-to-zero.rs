// https://leetcode.com/problems/minimum-operations-to-reduce-x-to-zero/
//
// Find the maximum sub-array.
//
// To find the optimal solution for this problem, we need to think about finding the maximum
// sub-array rather than finding the minimum prefix/suffix, as revealed by the question's hints on
// LeetCode.
//
// Finding the maximum sub-array can be done in one pass due to the greedy nature of the algorithm.
//
// Complexity: runtime O(n), space O(1).

impl Solution {
    pub fn min_operations(nums: Vec<i32>, x: i32) -> i32 {
        let (mut left, mut right) = (0usize, 0usize);
        let mut rem_sum = nums.iter().sum::<i32>() - nums[0];
        let mut result = std::i32::MAX;

        while left < nums.len() {
            if rem_sum > x {
                if right == nums.len() - 1 {
                    break; // no other solutions can exist, as nums only has +ves, and rem_sum > x
                }

                right += 1;
                rem_sum -= nums[right];
                continue;
            }

            if rem_sum == x {
                result = std::cmp::min(result, (nums.len() - right - 1 + left) as i32);
            }
            rem_sum += nums[left];
            left += 1;
        }

        if result == std::i32::MAX {
            -1
        } else {
            result
        }
    }
}

// Time Limit Exceeded: Recursion.
//
// This solution is the obvious one, but is too slow to pass the tests.
// To find the fastest solution, the problem needs to be thought of as finding the maximum
// sub-array instead.
//
// Complexity: runtime O(2^n), space O(n) [recursion stack].

impl Solution {
    fn min_operations_impl(nums_rem: &[i32], x_rem: i32, num_ops: i32) -> i32 {
        match x_rem {
            0 => num_ops,
            x if x < 0 => -1,
            _ => {
                if nums_rem.is_empty() {
                    return -1;
                }

                let min_left = Solution::min_operations_impl(
                    &nums_rem[1..],
                    x_rem - nums_rem.first().unwrap(),
                    num_ops + 1,
                );
                let min_right = Solution::min_operations_impl(
                    &nums_rem[..nums_rem.len() - 1],
                    x_rem - nums_rem.last().unwrap(),
                    num_ops + 1,
                );

                match (min_left, min_right) {
                    (-1, r) => r,
                    (l, -1) => l,
                    (l, r) => std::cmp::min(l, r),
                }
            }
        }
    }

    pub fn min_operations(nums: Vec<i32>, x: i32) -> i32 {
        Solution::min_operations_impl(&nums[..], x, 0)
    }
}
