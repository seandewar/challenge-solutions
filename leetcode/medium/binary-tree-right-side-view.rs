// https://leetcode.com/problems/binary-tree-right-side-view
//
// Complexity: runtime O(n), space O(n).

use std::cell::RefCell;
use std::rc::Rc;

impl Solution {
    pub fn right_side_view(root: Option<Rc<RefCell<TreeNode>>>) -> Vec<i32> {
        let mut result = Vec::new();
        let mut queue = std::collections::VecDeque::new();
        if let Some(root) = root {
            queue.push_back(root.clone());
        }

        while !queue.is_empty() {
            let len = queue.len();
            for i in 0..len {
                if let Some(node) = queue.pop_front() {
                    let node = node.borrow();
                    // as we push the right-most node first, that node at each level has i == 0
                    if i == 0 {
                        result.push(node.val);
                    }
                    if let Some(right) = &node.right {
                        queue.push_back(right.clone());
                    }
                    if let Some(left) = &node.left {
                        queue.push_back(left.clone());
                    }
                }
            }
        }

        result
    }
}
