// https://leetcode.com/problems/swapping-nodes-in-a-linked-list
//
// Single-pass.
// Complexity: runtime O(n), space O(1).

class Solution {
public:
    ListNode* swapNodes(ListNode* const head, int k) const
    {
        ListNode *n1 = nullptr, *n2 = head;
        for (auto tmp = head; tmp; tmp = tmp->next) {
            if (k == 0)
                n2 = n2->next;
            else if (--k == 0)
                n1 = tmp;
        }
        swap(n1->val, n2->val);
        return head;
    }
};
