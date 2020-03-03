// https://leetcode.com/problems/intersection-of-two-linked-lists/
//
// This solution uses constant space. See the alternative solution for a more
// obvious linear space solution.
//
// This solution works by calculating the size of both lists, and then advancing
// the head of the larger list by the abs() difference between the sizes of both
// lists; this allows both heads to start at the same distance from the end of
// the potentially-intersecting list, allowing them to reach the intersection
// point at the same time when stepped together.
//
// See code comments for details.
//
// Do note that this solution fails if any of the lists are cyclic, as the size
// calculation will loop forever, but this leetcode question guarantees
// non-cyclic list inputs.
//
// Complexity: runtime O(n+m), space O(1).

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
    ListNode* getIntersectionNode(ListNode* headA, ListNode* headB) const
    {
        // first, compute the size of both lists
        size_t sizeA = 0, sizeB = 0;

        {
            auto nodeA = headA, nodeB = headB;

            while (nodeA || nodeB) {
                if (nodeA) {
                    nodeA = nodeA->next;
                    ++sizeA;
                }

                if (nodeB) {
                    nodeB = nodeB->next;
                    ++sizeB;
                }
            }
        }

        // secondly, calculate the absolute difference of the list sizes
        // (abs(sizeA - sizeB)), and advance the head of the larger list by that
        // amount so that both head start at the same distance from the end of
        // the intersecting list
        {
            // NOTE: size_t is unsigned, so I calc the abs diff this way
            auto sizeDiff = max(sizeA, sizeB) - min(sizeA, sizeB);
            auto& largest = sizeA > sizeB ? headA : headB;

            while (sizeDiff-- > 0)
                largest = largest->next;
        }

        // now, if there is an intersection, both heads will reach that point at
        // the same time if we step them through the lists together. if there is
        // no intersection, one or both nodes will reach past the end of their
        // respective list
        while (headA && headB) {
            if (headA == headB)
                return headA;

            headA = headA->next;
            headB = headB->next;
        }

        return nullptr; // no intersection
    }
};

// Using a hash set.
// Complexity: runtime O(n+m), space O(n+m).
/*
class Solution {
public:
    ListNode* getIntersectionNode(ListNode* headA, ListNode* headB) const
    {
        unordered_set<const ListNode*> seen;

        while (headA || headB) {
            if (headA) {
                if (seen.count(headA) > 0)
                    return headA;

                seen.insert(headA);
                headA = headA->next;
            }

            if (headB) {
                if (seen.count(headB) > 0)
                    return headB;

                seen.insert(headB);
                headB = headB->next;
            }
        }

        return nullptr;
    }
};
*/
