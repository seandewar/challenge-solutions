// https://leetcode.com/problems/reduce-array-size-to-the-half
//
// Complexity: runtime O(n*logn), space O(n).

impl Solution {
    pub fn min_set_size(arr: Vec<i32>) -> i32 {
        let counts = arr.iter().fold(vec![0; 100_000], |mut acc, &x| {
            acc[x as usize - 1] += 1;
            acc
        });
        let mut heap = std::collections::BinaryHeap::from(counts);
        let (mut count, mut res) = (0, 0);
        while count * 2 < arr.len() {
            count += heap.pop().unwrap();
            res += 1;
        }
        res
    }
}

// Alternative Solution: Fun radix sort solution; more efficient theoretically, but less-so in
// practice, given the test inputs.
// Complexity: runtime O(n), space O(n).

impl Solution {
    pub fn min_set_size(arr: Vec<i32>) -> i32 {
        let mut counts = arr.iter().fold(vec![0; 100_000], |mut acc, &x| {
            acc[x as usize - 1] += 1;
            acc
        });
        Self::radix_sort_base10(&mut counts);
        counts
            .into_iter()
            .rev()
            .scan(0, |sum, x| {
                let ret = Some(*sum).filter(|&s| s * 2 < arr.len());
                *sum += x as usize;
                ret
            })
            .count() as _
    }

    fn radix_sort_base10(nums: &mut [i32]) {
        let mut buckets = vec![vec![]; 10];
        for i in 0..10 {
            nums.iter()
                .for_each(|&x| buckets[((x / 10_i32.pow(i)) % 10) as usize].push(x));
            buckets
                .iter()
                .flat_map(|b| b.iter())
                .zip(nums.iter_mut())
                .for_each(|(&x, y)| *y = x);
            buckets.iter_mut().for_each(|b| b.clear());
        }
    }
}
