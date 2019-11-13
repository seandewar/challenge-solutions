// https://leetcode.com/problems/linked-list-cycle-ii/
//
// Uses Floyd's cycle-finding (Floyd's Tortoise & Hare) algorithm.
//
// Using the algorithm, we have a "fast" and "slow" pointer, where the "slow"
// pointer traverses the list 1 step at a time and the "fast" pointer traverses
// 2 steps at a time. If both pointers eventually point to the same element in
// the list, we have a cycle. Otherwise, if the "fast" pointer reaches the end
// of the list, then there cannot be a cycle in the list.
//
// To find the start of the found cycle, define a 2nd "slow" pointer at the
// beginning of the list, and traverse both "slow" pointers through the list
// (again, 1 step at a time) until they eventually meet; this meeting point is
// where the cycle begins.
//
// The reason this algorithm works is as follows:
//
// During the cycle detection phase (phase 1), a net distance of 1 is created
// between the "fast" and "slow" pointers during each iteration, with the
// "fast" pointer reaching the cycle before the "slow" pointer. Once both
// pointers are inside of the cycle, the net distance "increase" between the
// pointers will instead cause a net distance decrease, with the pointers
// becoming 1 node closer due to the circular "wrapping" nature of the cycle.
// Eventually, the pointers will meet inside of the cycle, leading to phase 2.
//
// During the cycle finding phase (phase 2), traversing a 2nd "slow" pointer
// and the "slow" pointer within the cycle through the list will eventually
// have them meet at the node beginning the cycle. To understand how this
// works, define:
//   HE = distance from the list head to the cycle beginning.
//   EM = distance from the cycle beginning to the point where the "fast" &
//        "slow" pointers meet.
//
// When the "slow" & "fast" pointers meet, they would have travelled:
//   "slow" = HE + EM nodes,
//   "fast" = HE + 2*EM + C nodes = 2*distance of "slow",
//   where C is the remaining length of the cycle (such that due to the
//   circular "wrapping" nature of the cycle, HE + EM + C = HE)
//
// Using this information:
// 2*(HE + EM) = HE + 2*EM + C,
// 2*HE + 2*EM = HE + 2*EM + C,
// (2*HE + 2*EM) - (HE - 2*EM) = C,
// C = HE
//
// Therefore, traversing the "slow" pointer another HE steps will refer to the
// beginning of the cycle. Similarly, traversing the 2nd "slow" pointer HE
// steps from the head of the list will also refer to the beginning of the
// cycle. Therefore, wherever these pointers meet during phase 2 is guaranteed
// to be at the beginning of the cycle, and will happen after HE steps.
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
public:
    ListNode* detectCycle(ListNode* head)
    {
        auto fast = head, slow = head;

        // detect cycle. if there is one, the fast & slow ptrs will eventually
        // meet inside of the cycle
        do {
            if (!fast || !fast->next)
                return {}; // will surpass list tail & escape; no cycle

            fast = fast->next->next;
            slow = slow->next;
        } while (fast != slow);

        // fast & slow ptrs have met inside of a cycle. find the beginning node
        // within the cycle and return it
        fast = head; // reuse fast ptr as a 2nd slow ptr
        while (fast != slow) {
            fast = fast->next;
            slow = slow->next;
        }

        return slow;
    }
};

// Alternative Solution: using a hash set to store visited nodes while
// traversing the list; if a node is already in our set, we have already
// visited that node, indicating the start of a cycle!
//
// Complexity: runtime O(n), space O(n).
/*
class Solution {
public:
    ListNode* detectCycle(ListNode* head)
    {
        unordered_set<ListNode*> visitedNodes;

        while (head) {
            if (visitedNodes.count(head) > 0)
                return head;

            visitedNodes.insert(head);
            head = head->next;
        }

        return nullptr;
    }
};
*/
