// https://leetcode.com/problems/odd-even-linked-list/
//
// The idea is to use and modify the nodes of the input list to create two other
// lists: one containing the odd-indexed nodes and the other containing the
// even-indexed nodes. After the lists have been populated, join the end of the
// odd list together with the head of the even.
//
// Complexity: runtime O(n), space O(1).

class Solution {
public:
    ListNode* oddEvenList(ListNode* const head) const
    {
        if (!head || !head->next)
            return head;

        const auto evenHead = head->next; // 2nd node always head of even list
        auto oddEnd = head, evenEnd = head->next;

        while (evenEnd && evenEnd->next) {
            oddEnd = oddEnd->next = evenEnd->next;
            evenEnd = evenEnd->next = evenEnd->next->next;
        }

        oddEnd->next = evenHead; // join the lists together
        return head;
    }
};
