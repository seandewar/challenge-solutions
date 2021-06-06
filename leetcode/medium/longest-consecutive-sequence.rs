// https://leetcode.com/problems/longest-consecutive-sequence
//
// HashSet Solution.
// Complexity: runtime O(n), space O(n).

impl Solution {
    pub fn longest_consecutive(nums: Vec<i32>) -> i32 {
        let set: std::collections::HashSet<_> = nums.into_iter().collect();
        set.iter()
            .filter(|&&x| !set.contains(&(x - 1)))
            .map(|&x| (x..).take_while(|x| set.contains(x)).count())
            .max()
            .unwrap_or(0) as _
    }
}

// Alternative Solution: Radix sort (base 10).
// Complexity: runtime O(n), space O(n).

impl Solution {
    pub fn longest_consecutive(mut nums: Vec<i32>) -> i32 {
        Self::radix_sort_base10(&mut nums);
        nums.iter()
            .zip(nums.iter().skip(1))
            .scan(1, |n, (&a, &b)| {
                *n = if a + 1 >= b { *n + b - a } else { 1 };
                Some(*n)
            })
            .max()
            .unwrap_or(nums.len().min(1) as _)
    }

    fn radix_sort_base10(nums: &mut [i32]) {
        let mut buckets = vec![vec![]; 20];
        for i in 0..10 {
            nums.iter()
                .for_each(|&x| buckets[10 + ((x / 10_i32.pow(i)) % 10) as usize].push(x));
            buckets
                .iter()
                .flat_map(|b| b.iter())
                .zip(nums.iter_mut())
                .for_each(|(&x, y)| *y = x);
            buckets.iter_mut().for_each(|b| b.clear());
        }
    }
}
