// https://leetcode.com/problems/range-sum-query-mutable
//
// Similar to the C++ solution, but just use buckets of size 512.
// Also possible to use a Binary Indexed Tree here, or Segment Tree instead.
//
// Complexity: new() runtime O(n)
//             update() runtime O(1)
//             sum_range() runtime O(n/512), which is still technically O(n), but practically
//                                           faster than that given the test inputs
//             space O(n).

const BUCKET_LEN: usize = 512;

struct NumArray {
    buckets: Vec<[i32; BUCKET_LEN]>,
}

impl NumArray {
    fn new(nums: Vec<i32>) -> Self {
        let buckets = nums
            .chunks(BUCKET_LEN)
            .map(|c| {
                let mut bucket = [0; BUCKET_LEN];
                c.iter().enumerate().for_each(|(i, &x)| bucket[i] = x);
                (1..bucket.len()).for_each(|i| bucket[i] += bucket[i - 1]);
                bucket
            })
            .collect();

        Self { buckets }
    }

    fn idx(index: i32) -> (usize, usize) {
        (index as usize / BUCKET_LEN, index as usize % BUCKET_LEN)
    }

    fn update(&mut self, index: i32, val: i32) {
        let (b, i) = Self::idx(index);
        let bucket = &mut self.buckets[b];
        let new_val = bucket.get(i - 1).copied().unwrap_or(0) + val;
        let diff = new_val - std::mem::replace(&mut bucket[i], new_val);
        bucket.iter_mut().skip(i + 1).for_each(|x| *x += diff);
    }

    fn sum_range(&self, left: i32, right: i32) -> i32 {
        let (l, i) = Self::idx(left);
        let (r, j) = Self::idx(right);
        let sum_between: i32 = self.buckets[l..r]
            .iter()
            .map(|b| b.last().copied().unwrap())
            .sum();
        let right_sum = self.buckets[r][j];
        let left_extra = self.buckets[l].get(i - 1).copied().unwrap_or(0);

        sum_between + right_sum - left_extra
    }
}
