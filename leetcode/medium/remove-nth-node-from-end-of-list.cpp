// https://leetcode.com/problems/remove-nth-node-from-end-of-list/
//
// Uses 2 pointers and 1 pass of the list to delete an element N nodes away
// from the list end. Functionally equivalent to 1 pointer with 2 passes,
// albeit this is slightly better on cache locality.
//
// Complexity: runtime O(n), space O(1).

/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(NULL) {}
 * };
 */
class Solution {
public:
    ListNode* removeNthFromEnd(ListNode* head, int n)
    {
        // advance the fast ptr n steps ahead of slow
        auto fast{head};
        while (n-- > 0)
            fast = fast->next;

        // if n corresponds to the list head, fast will be null (fast is 1
        // place past the list's end: n == list size). delete the head
        if (!fast) {
            const auto newHead = head->next;
            delete head;
            return newHead;
        }

        // advance both ptrs until fast reaches the end of the list
        auto slow{head};
        while (fast->next) {
            fast = fast->next;
            slow = slow->next;
        }

        // slow ptr points to the node before the one to be deleted
        const auto nodeToDelete = slow->next;
        slow->next = nodeToDelete->next;
        delete nodeToDelete;
        return head;
    }
};
