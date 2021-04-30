// https://leetcode.com/problems/remove-element
//
// Complexity: runtime O(n), space O(1).

impl Solution {
    pub fn remove_element(nums: &mut Vec<i32>, val: i32) -> i32 {
        let (mut len, mut i) = (nums.len(), 0);
        while i < len {
            if nums[i] == val {
                nums.swap(i, len - 1);
                len -= 1;
            } else {
                i += 1;
            }
        }
        len as _
    }
}
