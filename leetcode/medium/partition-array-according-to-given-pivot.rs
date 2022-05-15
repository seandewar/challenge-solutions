// https://leetcode.com/problems/partition-array-according-to-given-pivot
// Complexity: runtime O(n), space O(n) including output, O(1) otherwise.

impl Solution {
    pub fn pivot_array(nums: Vec<i32>, pivot: i32) -> Vec<i32> {
        nums.iter()
            .filter(|&&x| x < pivot)
            .chain(nums.iter().filter(|&&x| x == pivot))
            .chain(nums.iter().filter(|&&x| x > pivot))
            .copied()
            .collect()
    }
}

// Alternative Solution: More imperative.
// Complexity: same as above.

impl Solution {
    pub fn pivot_array(nums: Vec<i32>, pivot: i32) -> Vec<i32> {
        let mut result = vec![0; nums.len()];

        let mut l = 0;
        let mut m = nums.iter().filter(|&&x| x < pivot).count();
        let mut r = nums.iter().filter(|&&x| x <= pivot).count();

        for v in nums {
            let i = if v < pivot {
                &mut l
            } else if v == pivot {
                &mut m
            } else {
                &mut r
            };

            result[*i] = v;
            *i += 1;
        }

        result
    }
}
