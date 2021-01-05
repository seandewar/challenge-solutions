// https://leetcode.com/problems/merge-two-sorted-lists/
//
// Iterative solution using a dummy root node and std::exchange() to simplify
// logic.
//
// Complexity: runtime O(m+n), space O(1).

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
    ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) const noexcept
    {
        ListNode fakeRoot(0, l1);
        l1 = &fakeRoot;

        while (l2) {
            if (!l1->next || l1->next->val > l2->val) {
                l2 = exchange(l2->next, exchange(l1->next, l2));
            } else {
                l1 = l1->next;
            }
        }

        return fakeRoot.next;
    }
};

// Alternative Solution: Like merge-two-sorted-lists.java's solution.
// Complexity: runtime O(m+n), space O(1).

class Solution {
public:
    ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) const noexcept
    {
        if (!l1 || !l2)
            return l1 ? l1 : l2;

        ListNode* root = nullptr;
        ListNode* endl = nullptr;

        while (l1 && l2) {
            const auto maxl = l1->val > l2->val ? l1 : l2;
            const auto minl = l1 == maxl ? l2 : l1;

            if (endl)
                endl = endl->next = minl;
            else
                root = endl = minl;

            l1 = minl->next;
            l2 = maxl;
        }

        endl->next = l1 ? l1 : l2;
        return root;
    }
};
