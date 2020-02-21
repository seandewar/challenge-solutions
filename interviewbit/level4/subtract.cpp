// https://www.interviewbit.com/problems/subtract/
//
// Uses in-place list reversal for the 2nd half of the list, allowing us to
// iterate that half in reverse order. The 2nd half of the list is restored to
// its initial state afterwards. See my LeetCode "Reverse Linked List" (easy)
// solution for more details.
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
// returns the new head of the reversed list
ListNode* reverseList(ListNode* root)
{
    ListNode* prev = nullptr;

    while (root) {
        const auto next = root->next;
        root->next = prev;

        prev = root;
        root = next;
    }

    return prev;
}

ListNode* Solution::subtract(ListNode* const root)
{
    if (!root)
        return nullptr;

    auto secondHalf = root;

    { // seek secondHalf to the beginning of the other half of the list
        auto fast = root;

        while (fast && fast->next) {
            secondHalf = secondHalf->next;
            fast = fast->next->next;
        }

        // for lists with an odd size, the middle node will have no
        // corresponding node to subtract against; for this reason, the 2nd half
        // of the list begins from the node afterwards.
        //
        // fast will not be null at the end of the loop in such a list
        if (fast)
            secondHalf = secondHalf->next;
    }

    // reverse the 2nd half of the list so we can traverse it in reverse order
    secondHalf = reverseList(secondHalf);

    { // perform the subtractions for the 1st half of the list against the 2nd
        auto first = root, second = secondHalf;

        while (second) {
            first->val = second->val - first->val;

            first = first->next;
            second = second->next;
        }
    }

    // reverse the 2nd half of the list again so we can restore it
    reverseList(secondHalf);

    return root;
}
