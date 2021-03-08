// https://leetcode.com/problems/convert-binary-number-in-a-linked-list-to-integer
//
// Complexity: runtime O(32) == O(1), space O(1).

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
    int getDecimalValue(const ListNode* head) const noexcept
    {
        int result = 0;
        for (; head; head = head->next) {
            result <<= 1;
            result |= head->val;
        }

        return result;
    }
};
