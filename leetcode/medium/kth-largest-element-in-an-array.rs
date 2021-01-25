// https://leetcode.com/problems/kth-largest-element-in-an-array
//
// Binary search for k.
//
// See my InterviewBit "Kth-Smallest Element in the Array" solution for more
// details on this solution.
//
// Complexity: runtime O(n*logk), space O(1).

impl Solution {
    pub fn find_kth_largest(nums: Vec<i32>, k: i32) -> i32 {
        let mut left = *nums.iter().min().unwrap();
        let mut right = *nums.iter().max().unwrap();

        while right >= left {
            let middle = left + (right - left) / 2;
            let gt = nums.iter().filter(|&&x| x > middle).count();
            let eq = nums.iter().filter(|&&x| x == middle).count();

            if gt <= k as usize - 1 && gt + eq >= k as usize {
                return middle;
            } else if gt > k as usize - 1 {
                left = middle + 1;
            } else {
                right = middle - 1;
            }
        }

        unreachable!("k should be valid in test inputs");
    }
}

// Alternative Solution: (Cheese) Sort array to find k.
// Can't do it in-place like the C++ version of this solution, as nums is immutable.
//
// Complexity: runtime O(n*logn), space O(n).

impl Solution {
    pub fn find_kth_largest(nums: Vec<i32>, k: i32) -> i32 {
        let mut nums_sorted = nums.clone();
        nums_sorted.sort();
        nums_sorted[nums_sorted.len() - k as usize]
    }
}
