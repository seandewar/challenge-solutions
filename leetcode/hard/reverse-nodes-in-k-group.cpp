// https://leetcode.com/problems/reverse-nodes-in-k-group
//
// Complexity: runtime O(n), space O(1).

class Solution {
public:
    ListNode* reverseKGroup(ListNode* head, const int k) const noexcept
    {
        ListNode *node = head, *prevOldGroupHead = nullptr;
        while (node) {
            int len = 0;
            for (auto tmp = node; tmp && len < k; tmp = tmp->next)
                ++len;
            if (len != k)
                break;
            ListNode *prev = nullptr, *const oldGroupHead = node;
            for (int i = 0; i < k && node; ++i)
                prev = exchange(node, exchange(node->next, prev));
            oldGroupHead->next = node;
            (prevOldGroupHead ? prevOldGroupHead->next : head) = prev;
            prevOldGroupHead = oldGroupHead;
        }
        return head;
    }
};
