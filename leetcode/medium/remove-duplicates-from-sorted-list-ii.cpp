// https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
//
// Iterative solution using a dummy root node to simplify cases where the
// original root may be deleted (e.g: duplicates of the root node).
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
    ListNode* deleteDuplicates(ListNode* head) const noexcept
    {
        ListNode fakeRoot(0, head);
        head = &fakeRoot; // head starts at dummy root

        while (head->next && head->next->next) {
            // find the node AFTER the last duplicate node of head->next
            auto dEnd = head->next->next;
            for (; dEnd && head->next->val == dEnd->val; dEnd = dEnd->next);

            // if no duplicates ahead, advance head; otherwise, set head->next
            // to node AFTER the end of the sub-list of duplicate nodes
            if (dEnd == head->next->next)
                head = head->next;
            else
                head->next = dEnd;
        }

        return fakeRoot.next;
    }
};
