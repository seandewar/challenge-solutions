// https://leetcode.com/problems/merge-two-sorted-lists/
//
// Iterative solution.
// See the C++ version of this solution for a "cooler" alternative.
//
// Complexity: runtime O(m+n), space O(1).

/**
 * Definition for singly-linked list.
 * public class ListNode {
 *     int val;
 *     ListNode next;
 *     ListNode(int x) { val = x; }
 * }
 */
class Solution {
    public ListNode mergeTwoLists(ListNode l1, ListNode l2) {
        // to eliminate the need for this check, you can instead assign a dummy
        // node to mergedHead and return mergedHead.next at the method's end
        if (l1 == null || l2 == null)
            return l1 == null ? l2 : l1;

        ListNode mergedHead = null;
        ListNode mergedEnd = null;

        while (l1 != null && l2 != null) {
            final var minNode = l1.val < l2.val ? l1 : l2;
            final var maxNode = minNode == l1 ? l2 : l1; // is the other list

            if (mergedEnd == null)
                mergedHead = mergedEnd = minNode;
            else
                mergedEnd = mergedEnd.next = minNode;

            // l1 may now refer to a node in l2 and vice versa, but this has no
            // effect on the final result, and simplifies logic a little
            l1 = minNode.next;
            l2 = maxNode;
        }

        mergedEnd.next = l1 == null ? l2 : l1;
        return mergedHead;
    }
}

// Alternative Solution: Recursive solution.
// Complexity: runtime O(m+n), space O(m+n).
/*
class Solution {
    public ListNode mergeTwoLists(final ListNode l1, final ListNode l2) {
        if (l1 == null || l2 == null)
            return l1 == null ? l2 : l1;

        final var minNode = l1.val < l2.val ? l1 : l2;

        // advance to the node after our now sorted min node in whatever list
        // it is in. the line below is equivalent to:
        // mergeTwoLists(minNode == l1 ? l1.next : l1,
        //               minNode == l2 ? l2.next : l2);
        // ...but it is less code and it may supply l1 as l2 or vice versa for
        // the next call, which has no effect on the final result
        minNode.next = mergeTwoLists(minNode.next, minNode == l1 ? l2 : l1);
        return minNode;
    }
}
*/
