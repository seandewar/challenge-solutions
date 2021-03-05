// https://leetcode.com/problems/average-levels-in-binary-tree
//
// Complexity: runtime O(n), space O(m) [where m is the maximum level width].

use std::cell::RefCell;
use std::collections::VecDeque;
use std::rc::Rc;

impl Solution {
    pub fn average_of_levels(root: Option<Rc<RefCell<TreeNode>>>) -> Vec<f64> {
        let (mut queue, mut res) = (VecDeque::new(), Vec::new());
        if let Some(root) = root {
            queue.push_back(root);
        }

        while !queue.is_empty() {
            let len = queue.len();
            let sum: i64 = (0..len)
                .map(|_| {
                    let node = queue.pop_front().unwrap();
                    let node = node.borrow();
                    if let Some(ref left) = node.left {
                        queue.push_back(Rc::clone(left));
                    }
                    if let Some(ref right) = node.right {
                        queue.push_back(Rc::clone(right));
                    }

                    node.val as i64
                })
                .sum();

            res.push(sum as f64 / len as f64);
        }

        res
    }
}
