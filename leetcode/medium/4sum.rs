// https://leetcode.com/problems/4sum
//
// Complexity: runtime O((n^3)*logn), space O(logn) [from sort_unstable()].

impl Solution {
    pub fn four_sum(mut nums: Vec<i32>, target: i32) -> Vec<Vec<i32>> {
        let mut res = vec![];
        nums.sort_unstable();
        for i in 0..nums.len() {
            if i != 0 && nums[i] == nums[i - 1] {
                continue;
            }
            for j in i + 1..nums.len() {
                if j != i + 1 && nums[j] == nums[j - 1] {
                    continue;
                }
                for k in j + 1..nums.len() {
                    if k != j + 1 && nums[k] == nums[k - 1] {
                        continue;
                    }
                    let rem = target - nums[i] - nums[j] - nums[k];
                    if nums[k + 1..].binary_search(&rem).is_ok() {
                        res.push(vec![nums[i], nums[j], nums[k], rem]);
                    }
                }
            }
        }
        res
    }
}

// Alternative Solution: Recursive approach; can be transformed into an iterative approach pretty
// easily.
//
// Complexity: runtime O((n^3)*logn), space O(logn) [from sort_unstable()].

impl Solution {
    pub fn four_sum(mut nums: Vec<i32>, target: i32) -> Vec<Vec<i32>> {
        nums.sort_unstable();
        let (mut tmp, mut res) = (Vec::with_capacity(4), vec![]);
        Self::solve(&nums[..], target, &mut tmp, &mut res);
        res
    }

    fn solve(nums: &[i32], target: i32, cur: &mut Vec<i32>, out: &mut Vec<Vec<i32>>) {
        if !nums.is_empty() && cur.len() < 3 {
            let i = nums
                .iter()
                .position(|&x| x != nums[0])
                .unwrap_or(nums.len());
            let sliced = &nums[0.max(i as isize + cur.len() as isize - 4) as usize..];
            cur.push(sliced[0]);
            Self::solve(&sliced[1..], target - sliced[0], cur, out);
            cur.pop();
            Self::solve(&nums[i..], target, cur, out);
        } else if nums.binary_search(&target).is_ok() {
            cur.push(target);
            out.push(cur.clone());
            cur.pop();
        }
    }
}
