// https://leetcode.com/problems/deepest-leaves-sum
//
// Complexity: runtime O(n), space O(n) [O(logn) if balanced].

use std::cell::RefCell;
use std::rc::Rc;

impl Solution {
    pub fn deepest_leaves_sum(root: Option<Rc<RefCell<TreeNode>>>) -> i32 {
        fn dfs(
            node: Option<Rc<RefCell<TreeNode>>>,
            depth: i32,
            max_depth: &mut i32,
            sum: &mut i32,
        ) {
            if let Some(node) = node {
                if depth > *max_depth {
                    *max_depth = depth;
                    *sum = 0;
                }

                let node = node.borrow();
                if depth == *max_depth {
                    *sum += node.val;
                }

                dfs(node.left.clone(), depth + 1, max_depth, sum);
                dfs(node.right.clone(), depth + 1, max_depth, sum);
            }
        }

        let (mut max_depth, mut sum) = (0, 0);
        dfs(root, 0, &mut max_depth, &mut sum);

        sum
    }
}
