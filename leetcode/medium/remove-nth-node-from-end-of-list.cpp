// https://leetcode.com/problems/remove-nth-node-from-end-of-list/
//
// Uses 2 pointers and 1 pass of the list to delete an element N nodes away
// from the list end. Functionally equivalent to 1 pointer with 2 passes,
// albeit this is slightly better on cache locality.
//
// "fast" has a n step head start in front of our slow pointer so that our slow
// pointer refers to the node to delete after "fast" hits the end of the list
// (our slow pointer starts at a node before the original head, so that we can
// change the node before the node to delete to refer to the node after the
// deleted node as the next node).
//
// "head" is re-used as the "slow" pointer.
//
// Complexity: runtime O(n), space O(1).

class Solution {
public:
    ListNode* removeNthFromEnd(ListNode* head, int n)
    {
        ListNode beforeHead(0, head), *fast = head;
        for (; n > 0; --n)
            fast = fast->next;
        for (head = &beforeHead; fast; fast = fast->next)
            head = head->next;
        delete exchange(head->next, head->next->next);
        return beforeHead.next;
    }
};
