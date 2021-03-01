// https://leetcode.com/problems/sort-an-array
//
// Merge Sort.
// Complexity: runtime O(nlogn), space O(n) [O(logn) excluding output].

impl Solution {
    fn merge_sort<T: Clone + Ord>(range: &[T]) -> Vec<T> {
        if range.len() < 2 {
            return range.into();
        }

        let mut a = Solution::merge_sort(&range[..range.len() / 2]);
        let mut b = Solution::merge_sort(&range[range.len() / 2..]);
        let mut res = Vec::with_capacity(range.len());

        let (mut i, mut j) = (0, 0);
        while i < a.len() || j < b.len() {
            if j >= b.len() || i < a.len() && a[i] < b[j] {
                res.push(a[i].clone());
                i += 1;
            } else {
                res.push(b[j].clone());
                j += 1;
            }
        }

        res
    }

    pub fn sort_array(nums: Vec<i32>) -> Vec<i32> {
        Solution::merge_sort(&nums[..])
    }
}

// Alternative Solution: In-place Merge Sort. (n^2 runtime due to the in-place merge!)
// Complexity: runtime O(n^2), space O(logn) [call stack].

impl Solution {
    fn merge_sort_inplace<T: Ord>(range: &mut [T]) {
        let mut a_len = range.len() / 2;
        let mut b_len = range.len() - a_len;

        if (range.len() > 1) {
            Solution::merge_sort_inplace(&mut range[..a_len]);
            Solution::merge_sort_inplace(&mut range[a_len..]);
        }

        let (mut i, mut j) = (0, 0);
        while i < a_len || j < b_len {
            if j >= b_len || i < a_len && range[i] < range[a_len + j] {
                range.swap(i + j, i);
                i += 1;
            } else {
                for k in (i + j)..(a_len + j) {
                    range.swap(k, a_len + j);
                }

                i += 1;
                a_len += 1;
                b_len -= 1;
            }
        }
    }

    pub fn sort_array(mut nums: Vec<i32>) -> Vec<i32> {
        Solution::merge_sort_inplace(&mut nums[..]);
        nums
    }
}
