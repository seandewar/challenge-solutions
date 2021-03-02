// https://leetcode.com/problems/set-mismatch
//
// HashSet to find duplicate num, XOR to find the missing num.
// Complexity: runtime O(n), space O(n).

impl Solution {
    pub fn find_error_nums(nums: Vec<i32>) -> Vec<i32> {
        let miss_xor_dup = nums
            .iter()
            .enumerate()
            .fold(0, |acc, (i, &x)| acc ^ (i as i32 + 1) ^ x);

        let mut set = std::collections::HashSet::new();
        nums.into_iter()
            .find(|x| !set.insert(x))
            .map(|x| vec![x, miss_xor_dup ^ x])
            .unwrap()
    }
}

// Alternative Solution: XOR + Negate nums[x - 1] for each x in nums to isolate the duplicate.
//
// By doing this, the duplicate will be positive after the traversal.
// We can negate as the values in nums are > 0.
//
// Complexity: runtime O(n), space O(1).

impl Solution {
    pub fn find_error_nums(mut nums: Vec<i32>) -> Vec<i32> {
        let (miss_xor_dup, dup) = (0..nums.len()).fold((0, None), |(xor, dup), i| {
            let (x, y) = (nums[i].abs(), i as i32 + 1);
            let mark_i = x as usize - 1;
            nums[mark_i] *= -1;

            (xor ^ y ^ x, dup.or(Some(x).filter(|_| nums[mark_i] > 0)))
        });

        vec![dup.unwrap(), miss_xor_dup ^ dup.unwrap()]
    }
}

// Alternative Solution: Sorting + Iterator fun.
// Complexity: runtime O(n*logn), space O(logn).

impl Solution {
    pub fn find_error_nums(mut nums: Vec<i32>) -> Vec<i32> {
        nums.sort_unstable();

        let last_dup_idx = (1..nums.len()).find(|&i| nums[i] == nums[i - 1]).unwrap();
        let miss = (1..=last_dup_idx as i32)
            .chain(last_dup_idx as i32..nums.len() as _)
            .zip(nums.iter())
            .find(|&(expect, &x)| expect != x)
            .map(|(miss, _)| miss)
            .unwrap_or(nums.len() as _);

        vec![nums[last_dup_idx], miss]
    }
}
