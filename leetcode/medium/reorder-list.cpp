// https://leetcode.com/problems/reorder-list/
//
// In-place modification.
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
    void reorderList(ListNode* const head) const noexcept
    {
        auto left = head, right = head;

        // seek the right ptr to the half-way point in the list (+1 for
        // odd-sized lists, so the right side is smaller), and split the list at
        // that point
        {
            ListNode *fast = head, *prev = nullptr;

            while (fast) {
                // allows us to move the right ptr 1 extra step if the list's
                // size is odd
                fast = fast->next ? fast->next->next : nullptr;

                // the rest handles the updating of the right ptr and the
                // splitting of the list by initially updating the next ptr of
                // each node to null, restoring it on the next iteration of the
                // loop; this has the effect of leaving the next ptr of the
                // middle node as null when the loop ends
                if (prev) {
                    prev->next = right;
                }

                prev = exchange(right, exchange(right->next, nullptr));
            }
        }

        // reverse the list from right to end, so right can be used to iterate
        // through the end of the list backwards
        {
            ListNode* prev = nullptr;

            while (right) {
                prev = exchange(right, exchange(right->next, prev));
            }

            right = prev; // right will point to past-the-end, so step back
        }

        // re-order the list
        while (right) {
            left = exchange(left->next, right);
            right = exchange(right->next, left);
        }
    }
};
