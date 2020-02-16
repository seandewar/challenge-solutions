// https://leetcode.com/problems/add-two-numbers/
//
// Complexity: runtime O(m+n), space O(1) [output not considered].

/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(NULL) {}
 * };
 */
class Solution {
public:
    ListNode* addTwoNumbers(const ListNode* l1, const ListNode* l2) const
    {
        ListNode* head = nullptr, end = nullptr;
        bool carry = false;

        while (l1 || l2 || carry) {
            const int digitSum = (l1 ? l1->val : 0) + (l2 ? l2->val : 0)
                                 + (carry ? 1 : 0);
            carry = digitSum > 9;

            const auto newNode = new ListNode(digitSum % 10);

            if (!head)
                head = end = newNode;
            else
                end = end->next = newNode;

            l1 = l1 ? l1->next : nullptr;
            l2 = l2 ? l2->next : nullptr;
        }

        return head;
    }
};

// Alternative Solution: shorter version (slightly harder to read).
//
// Many ways to make this even shorter (e.g: comma expressions), but this isn't
// code golf (or is it?)
//
// Complexity: runtime O(m+n), space O(1) [output not considered].
/*
class Solution {
public:
    ListNode* addTwoNumbers(const ListNode* l1, const ListNode* l2) const
    {
        ListNode* head = nullptr, * end = nullptr;
        int accum = 0;

        while (l1 || l2 || accum > 0) {
            accum += (l1 ? l1->val : 0) + (l2 ? l2->val : 0);
            const auto newNode = new ListNode(accum % 10);

            if (!head)
                head = end = newNode;
            else
                end = end->next = newNode;

            accum /= 10;
            l1 = l1 ? l1->next : nullptr;
            l2 = l2 ? l2->next : nullptr;
        }

        return head;
    }
};
*/
