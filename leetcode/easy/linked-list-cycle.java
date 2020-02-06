// https://leetcode.com/problems/linked-list-cycle/
//
// Uses Floyd's "Hare & Tortoise" Cycle Detection algorithm, using O(1) space.
//
// See my Linked List Cycle II solution (leetcode medium difficulty) for more
// details on the algorithm (including how to find the start of the cycle
// afterwards [without using extra memory]).
//
// Complexity: runtime O(n), space O(1).

/**
 * Definition for singly-linked list.
 * class ListNode {
 *     int val;
 *     ListNode next;
 *     ListNode(int x) {
 *         val = x;
 *         next = null;
 *     }
 * }
 */
public class Solution {
    public boolean hasCycle(final ListNode head) {
        ListNode fast = head, slow = head; // fast & slow begin at the head

        while (fast != null && fast.next != null) {
            fast = fast.next.next; // fast ptr moves 2 steps
            slow = slow.next;      // slow ptr moves 1 step

            if (fast == slow) // must be cyclic if fast met up with slow again;
                return true;  // fast must have ended up behind slow!
        }

        return false; // reached end of list, therefore it cannot be cyclic
    }
}

// Alternative Solution: using a hash set.
// Complexity: runtime O(n), space O(n).
/*
public class Solution {
    public boolean hasCycle(ListNode head) {
        final var visitedSet = new HashSet<ListNode>();

        while (head != null) {
            if (visitedSet.contains(head))
                return true;

            visitedSet.add(head);
            head = head.next;
        }

        return false;
    }
}
*/
