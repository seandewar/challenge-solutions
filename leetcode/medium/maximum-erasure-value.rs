// https://leetcode.com/problems/maximum-erasure-value
//
// Using HashSet. Can be written more succinctly when not using these iterator methods, but they
// look cool. :)
//
// Complexity: runtime O(n), space O(n).

impl Solution {
    pub fn maximum_unique_subarray(nums: Vec<i32>) -> i32 {
        let mut set = std::collections::HashSet::new();
        let (max, cur_sum, _) =
            nums.iter()
                .fold((0, 0, 0), |(mut max, mut cur_sum, mut win_starti), x| {
                    if set.contains(x) {
                        max = max.max(cur_sum);
                        let (j, _) = nums
                            .iter()
                            .enumerate()
                            .skip(win_starti)
                            .find(|&(_, y)| x == y)
                            .unwrap();
                        nums[win_starti..=j].iter().for_each(|y| {
                            cur_sum -= y;
                            set.remove(y);
                        });
                        win_starti = j + 1;
                    }
                    set.insert(*x);
                    (max, cur_sum + x, win_starti)
                });
        max.max(cur_sum)
    }
}

// Alternative Solution: Using a HashMap.
// Complexity: runtime O(n), space O(n).

impl Solution {
    pub fn maximum_unique_subarray(nums: Vec<i32>) -> i32 {
        let mut map = std::collections::HashMap::new();
        let (max, cur_sum, _) = nums.iter().enumerate().fold(
            (0, 0, 0),
            |(mut max, mut cur_sum, mut win_starti), (i, x)| {
                if let Some(&j) = map.get(x) {
                    max = max.max(cur_sum);
                    nums[win_starti..=j].iter().for_each(|y| {
                        cur_sum -= y;
                        map.remove(y);
                    });
                    win_starti = j + 1;
                }
                map.insert(*x, i);
                (max, cur_sum + x, win_starti)
            },
        );
        max.max(cur_sum)
    }
}
