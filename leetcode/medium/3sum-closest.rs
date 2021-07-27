// https://leetcode.com/problems/3sum-closest
//
// Complexity: runtime O((n^2)*logn), space O(logn).

impl Solution {
    pub fn three_sum_closest(mut nums: Vec<i32>, target: i32) -> i32 {
        nums.sort_unstable();
        let mut res = nums[0] + nums[1] + nums[2];
        for i in 0..nums.len() {
            for j in i + 1..nums.len() {
                let (a, b) = (nums[i], nums[j]);
                let k = nums[j + 1..].binary_search(&(target - a - b));
                if let Ok(_) = k {
                    return target;
                } else if let Err(k) = k.map_err(|k| j + 1 + k) {
                    let (sum, dist) = (|c| a + b + c, |s: i32| (target - s).abs());
                    let (c1, c2) = (nums.get(k), nums.get(k - 1).filter(|_| k > j + 1));
                    let (s1, s2) = (c1.map(sum), c2.map(sum));
                    let (d1, d2) = (s1.map(dist), s2.map(dist));
                    let near_sum = if d2.is_none() || d1.map_or(false, |d1| d1 < d2.unwrap()) {
                        s1
                    } else {
                        s2
                    };
                    if let Some(s) = near_sum.filter(|&s| dist(s) < dist(res)) {
                        res = s;
                    }
                }
            }
        }
        res
    }
}
