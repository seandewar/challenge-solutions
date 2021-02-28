// https://leetcode.com/problems/equal-sum-arrays-with-minimum-number-of-operations
//
// Vomit, but fast.
//
// Essentially, we choose the smallest values in nums with the smallest sum to maximize and the
// largest values in the other nums to minimize. To avoid looping through the arrays each time,
// we just manipulate a frequency table for both nums instead.
//
// Complexity: runtime O(m+n), space O(1).

impl Solution {
    pub fn min_operations(nums1: Vec<i32>, nums2: Vec<i32>) -> i32 {
        let (sum1, sum2): (i32, i32) = (nums1.iter().sum(), nums2.iter().sum());
        let (hi, lo) = if sum1 > sum2 {
            (&nums1, &nums2)
        } else {
            (&nums2, &nums1)
        };

        let mut sum_diff = (sum1 - sum2).abs();
        let (mut hi_cnt, mut lo_cnt) = ([0; 6], [0; 6]);
        hi.iter().for_each(|&x| hi_cnt[x as usize - 1] += 1);
        lo.iter().for_each(|&x| lo_cnt[x as usize - 1] += 1);

        let mut res = 0;
        while sum_diff > 0 {
            let hi_max = hi_cnt
                .iter()
                .enumerate()
                .rev()
                .find(|&(_, &n)| n > 0)
                .map(|(i, _)| i + 1)
                .unwrap() as i32;
            let lo_min = lo_cnt
                .iter()
                .enumerate()
                .find(|&(_, &n)| n > 0)
                .map(|(i, _)| i + 1)
                .unwrap() as i32;

            let (hi_max_diff, lo_min_diff) = (hi_max - 1, 6 - lo_min);
            if hi_max_diff == 0 && lo_min_diff == 0 {
                return -1; // no more operations possible
            }

            if hi_max_diff > lo_min_diff {
                let ops = (sum_diff / hi_max_diff + (sum_diff % hi_max_diff).min(1))
                    .min(hi_cnt[hi_max as usize - 1]);
                res += ops;
                sum_diff -= ops * hi_max_diff;
                hi_cnt[0] += std::mem::replace(&mut hi_cnt[hi_max as usize - 1], 0);
            } else {
                let ops = (sum_diff / lo_min_diff + (sum_diff % lo_min_diff).min(1))
                    .min(lo_cnt[lo_min as usize - 1]);
                res += ops;
                sum_diff -= ops * lo_min_diff;
                lo_cnt[5] += std::mem::replace(&mut lo_cnt[lo_min as usize - 1], 0);
            }
        }

        res
    }
}
