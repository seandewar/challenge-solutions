// https://leetcode.com/problems/partition-list
//
// In-place.
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
    ListNode* partition(ListNode* head, int x) const
    {
        ListNode lessHead(0), moreHead(0), *less = &lessHead, *more = &moreHead;
        while (head) {
            auto& end = head->val < x ? less : more;
            end = end->next = exchange(head, exchange(head->next, nullptr));
        }
        less->next = moreHead.next;
        return lessHead.next;
    }
};
