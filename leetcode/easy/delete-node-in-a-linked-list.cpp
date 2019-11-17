// https://leetcode.com/problems/delete-node-in-a-linked-list/
//
// Deletes node by replacing node with node->next, then deletes the now
// duplicate node->next.
// Complexity: runtime O(1), space O(1).

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
    void deleteNode(ListNode* node)
    {
        auto next = node->next;
        *node = *next;
        delete next;
    }
};

// Alternative Solution: delete by overwriting the value of all nodes from node
// to the tail with the value of the next node, then delete the tail.
// Complexity: runtime O(n), space O(1).
/*
class Solution {
public:
    void deleteNode(ListNode* node)
    {
        ListNode* prev{};
        while (node->next) {
            node->val = node->next->val;
            prev = node;
            node = node->next;
        }

        prev->next = nullptr;
        delete node;
    }
};
*/
