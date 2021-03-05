// https://leetcode.com/problems/sum-nodes-with-even-valued-grandparent
//
// Complexity: runtime O(n), space O(n) [O(logn) if balanced].

use std::cell::RefCell;
use std::rc::Rc;

impl Solution {
    pub fn sum_even_grandparent(root: Option<Rc<RefCell<TreeNode>>>) -> i32 {
        fn dfs(node: Option<Rc<RefCell<TreeNode>>>, mut gp_counters: [isize; 2]) -> i32 {
            if let Some(node) = node {
                let node = node.borrow();
                gp_counters.iter_mut().for_each(|c| *c -= 1);

                let ret = if gp_counters.iter().any(|&c| c == 0) {
                    node.val
                } else {
                    0
                };
                if node.val % 2 == 0 {
                    *gp_counters.iter_mut().find(|&&mut c| c <= 0).unwrap() = 2;
                }

                return ret
                    + dfs(node.left.clone(), gp_counters)
                    + dfs(node.right.clone(), gp_counters);
            }

            0
        }

        dfs(root, [-1, -1])
    }
}
