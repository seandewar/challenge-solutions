// https://leetcode.com/problems/max-number-of-k-sum-pairs
//
// Simplified Hash Map implementation.
//
// It's also possible to implement this solution via sorting, without the hash map, if wanted.
//
// Same idea as the below alternative hash map solution, but simplified by only doing one pass
// through the nums, only incrementing the count of the difference needed to sum to k. When we come
// across a num in nums that has been previously marked as a needed difference to sum to k (has a
// count > 1 in needed_counts), we will have a completed pair, so we can increment the result!
//
// Complexity: runtime O(n), space O(n).

impl Solution {
    pub fn max_operations(nums: Vec<i32>, k: i32) -> i32 {
        let mut needed_counts = std::collections::HashMap::new();
        let mut result = 0;

        for x in nums {
            if let Some(count) = needed_counts.get_mut(&x) {
                if *count > 0 {
                    *count -= 1;
                    result += 1;
                    continue;
                }
            }
            *needed_counts.entry(k - x).or_insert(0) += 1;
        }

        result
    }
}

// Alternative Solution: Initial Hash Map implementation.
// Complexity: runtime O(n), space O(n).

impl Solution {
    pub fn max_operations(nums: Vec<i32>, k: i32) -> i32 {
        let mut nums_counts = std::collections::HashMap::new();
        for x in &nums {
            *nums_counts.entry(x).or_insert(0) += 1;
        }

        let mut result = 0;
        for x in &nums {
            let counts = (nums_counts.get(x), nums_counts.get(&(k - x)));
            if let (Some(x_count), Some(y_count)) = counts {
                let is_pair = {
                    if std::ptr::eq(x_count, y_count) {
                        *x_count > 1
                    } else {
                        *x_count > 0 && *y_count > 0
                    }
                };
                if is_pair {
                    *nums_counts.get_mut(x).unwrap() -= 1;
                    *nums_counts.get_mut(&(k - x)).unwrap() -= 1;
                    result += 1;
                }
            }
        }

        result
    }
}
