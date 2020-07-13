// https://leetcode.com/problems/same-tree/
//
// Complexity: runtime O(n), space O(n) [recursion overhead; single-branch tree of length n].

// Definition for a binary tree node.
// #[derive(Debug, PartialEq, Eq)]
// pub struct TreeNode {
//   pub val: i32,
//   pub left: Option<Rc<RefCell<TreeNode>>>,
//   pub right: Option<Rc<RefCell<TreeNode>>>,
// }
//
// impl TreeNode {
//   #[inline]
//   pub fn new(val: i32) -> Self {
//     TreeNode {
//       val,
//       left: None,
//       right: None
//     }
//   }
// }

use std::rc::Rc;
use std::cell::RefCell;

// blame leetcode for the terribly overcomplicated type; wtf?!
type NodeRefOption = Option<Rc<RefCell<TreeNode>>>;

impl Solution {
    pub fn is_same_tree(p: NodeRefOption, q: NodeRefOption) -> bool {
        match (p, q) {
            (Some(p_ref), Some(q_ref)) => {
                let (p_node, q_node) = (p_ref.borrow(), q_ref.borrow());

                p_node.val == q_node.val
                    && Self::is_same_tree(p_node.left.clone(), q_node.left.clone())
                    && Self::is_same_tree(p_node.right.clone(), q_node.right.clone())
            }
            (None, None) => true,
            _ => false,
        }
    }
}
