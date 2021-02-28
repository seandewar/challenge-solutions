// https://leetcode.com/problems/range-sum-of-bst
//
// Complexity: runtime O(n), space O(n) [O(logn) if balanced].

use std::cell::RefCell;
use std::rc::Rc;

impl Solution {
    pub fn range_sum_bst(root: Option<Rc<RefCell<TreeNode>>>, low: i32, high: i32) -> i32 {
        if let Some(root) = root {
            let (root, mut sum) = (root.borrow(), 0);
            if root.val >= low && root.val <= high {
                sum += root.val;
            }
            if root.val >= low {
                sum += Solution::range_sum_bst(root.left.clone(), low, high);
            }
            if root.val <= high {
                sum += Solution::range_sum_bst(root.right.clone(), low, high);
            }

            sum
        } else {
            0
        }
    }
}
