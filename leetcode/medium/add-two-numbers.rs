// https://leetcode.com/problems/add-two-numbers/
//
// Complexity: runtime O(m+n), space O(1).

// Definition for singly-linked list.
// #[derive(PartialEq, Eq, Clone, Debug)]
// pub struct ListNode {
//   pub val: i32,
//   pub next: Option<Box<ListNode>>
// }
//
// impl ListNode {
//   #[inline]
//   fn new(val: i32) -> Self {
//     ListNode {
//       next: None,
//       val
//     }
//   }
// }
impl Solution {
    pub fn add_two_numbers(
        l1: Option<Box<ListNode>>,
        l2: Option<Box<ListNode>>,
    ) -> Option<Box<ListNode>> {
        let mut result_fake_root = ListNode::new(0);
        let mut result_end = &mut result_fake_root;

        let (mut n1, mut n2) = (&l1, &l2);
        let mut carry = 0;

        while n1.is_some() || n2.is_some() || carry != 0 {
            let mut total = carry;
            if let Some(n) = n1 {
                total += n.val;
                n1 = &n.next;
            }
            if let Some(n) = n2 {
                total += n.val;
                n2 = &n.next;
            }

            result_end.next = Some(Box::new(ListNode::new(total % 10)));
            result_end = result_end.next.as_mut().unwrap();
            carry = total / 10;
        }

        result_fake_root.next
    }
}
