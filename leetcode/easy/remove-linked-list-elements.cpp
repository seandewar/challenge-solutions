// https://leetcode.com/problems/remove-linked-list-elements/
//
// Complexity: runtime O(n), space O(1).

/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */
class Solution {
public:
    ListNode* removeElements(ListNode* head, const int val) const
    {
        ListNode *curr = head, *prev = nullptr;

        while (curr) {
            if (const auto next = curr->next; curr->val == val) {
                (prev ? prev->next : head) = next;
                delete curr;
                curr = next;
            } else {
                prev = exchange(curr, next);
            }
        }

        return head;
    }
};
