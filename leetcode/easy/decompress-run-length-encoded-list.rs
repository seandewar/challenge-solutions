// https://leetcode.com/problems/decompress-run-length-encoded-list
//
// Complexity: runtime O(n), space O(n).

impl Solution {
    pub fn decompress_rl_elist(nums: Vec<i32>) -> Vec<i32> {
        assert!(nums.len() % 2 == 0);

        let mut res = Vec::new();
        for chunk in nums.chunks(2) {
            let (freq, val) = (chunk[0], chunk[1]);
            res.extend(std::iter::repeat(val).take(freq as _));
        }

        res
    }
}
