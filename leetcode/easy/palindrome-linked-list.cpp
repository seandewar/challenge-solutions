// https://leetcode.com/problems/palindrome-linked-list/
//
// Linear time solution (~2.5 list traversals) with no extra space.
//
// This solution mutates the list. You may want to consider undoing
// modifications to the list once the result is calculated, if you care enough.
//
// See the alternative solution for a more obvious and less lengthy O(n) extra
// space solution.
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
class Solution {
    auto computeListSize(const ListNode* head) const noexcept
    {
        size_t size{};

        while (head) {
            head = head->next;
            ++size;
        }

        return size;
    }

    // returns the new head of the now reversed list
    auto reverseList(ListNode* head) const noexcept -> ListNode*
    {
        ListNode* prev{};

        while (head) {
            const auto next{head->next};

            head->next = prev;
            prev = head;
            head = next;
        }

        return prev;
    }

public:
    auto isPalindrome(ListNode* head) const noexcept
    {
        const auto size{computeListSize(head)};
        auto node{head};

        // navigate to the 2nd half of the list. skip the middle node if the
        // size is odd (it doesn't affect the list's symmetry).
        //
        // alternatively, you may navigate to the 2nd half using a fast and
        // slow pointer (it will result in the same number of list accesses,
        // though)
        for (size_t i{}; i < size / 2 + size % 2; ++i)
            node = node->next;

        node = reverseList(node);

        for (size_t i{}; i < size / 2; ++i) {
            if (head->val != node->val)
                return false;

            head = head->next;
            node = node->next;
        }

        return true;
    }
};

// Alternative Solution: Short version of the above.
// Complexity: runtime O(n), space O(1).

class Solution {
public:
    bool isPalindrome(ListNode* head)
    {
        // find the half-way point of the list
        auto slow = head;
        for (auto fast = head; fast && fast->next; fast = fast->next->next)
            slow = slow->next;

        // reverse 2nd half of the list ("prev" will be the head of the reversed
        // half afterwards)
        ListNode* prev = nullptr;
        for (; slow; slow = exchange(slow->next, exchange(prev, slow)));

        // check both list halves for equality
        for (; prev; prev = prev->next)
            if (exchange(head, head->next)->val != prev->val)
                return false;

        return true;
    }
};

// Alternative Solution: using extra space.
// Complexity: runtime O(n), space O(n).

class Solution {
    auto computeListSize(const ListNode* head) const noexcept
    {
        size_t size{};

        while (head) {
            head = head->next;
            ++size;
        }

        return size;
    }

public:
    auto isPalindrome(const ListNode* head) const
    {
        vector<reference_wrapper<const int>> secondHalf;

        {
            const auto size{computeListSize(head)};
            auto node{head};

            // navigate to the 2nd half of the list. this method saves some
            // space (still O(n)), but alternatively, you may simply traverse
            // and store the whole list in such a way where you can iterate
            // through it in reverse order if you want to
            for (size_t i{}; i < size / 2 + size % 2; ++i)
                node = node->next;

            secondHalf.reserve(size / 2);

            for (size_t i{}; i < size / 2; ++i) {
                secondHalf.emplace_back(node->val);
                node = node->next;
            }
        }

        for (auto rit{crbegin(secondHalf)}; rit != crend(secondHalf); ++rit) {
            if (head->val != *rit)
                return false;

            head = head->next;
        }

        return true;
    }
};
