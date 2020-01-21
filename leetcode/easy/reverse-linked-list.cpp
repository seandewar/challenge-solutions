// https://leetcode.com/problems/reverse-linked-list/
//
// We simply reverse the direction of the next pointer. Because this isn't a
// doubly-linked list with previous pointers, we need to temporarily store the
// previous element ourselves.
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
    ListNode* reverseList(ListNode* head)
    {
        ListNode* prev{};

        while (head) {
            const auto next{head->next};
            head->next = prev;
            prev = head;
            head = next;
        }

        return prev;
    }
};

// Alternative Solution: Recursive version.
// Complexity: runtime O(n), space O(n).
/*
class Solution {
public:
    ListNode* reverseList(ListNode* head)
    {
        if (!head || !head->next)
            return head;

        const auto prev = reverseList(head->next);
        head->next->next = head;
        head->next = {};

        return prev;
    }
};
*/
