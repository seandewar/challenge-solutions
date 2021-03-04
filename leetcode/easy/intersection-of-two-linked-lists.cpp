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

// Alternative Solution: No mutation, constant space.
//
// In a list with an intersection point, there will be a number of nodes
// (possibly 0) before the intersection point for A & B. Therefore:
//
// From headA: nodesA + intersection + nodesAB
// From headB: nodesB + intersection + nodesAB
//
// If we traverse the lists from headA & headB, and restart the traversal from
// the opposite head once we reach the end, if there is an intersection, we will
// reach the intersection point at the same time after the restart! If there is
// no intersection, we will reach the end of the list at the same time.
//
// The reason for this is that regardless of the distance of nodesA or nodesB,
// both traversals will eventually step the same number of nodes once the
// restart takes place at the other head:
//
// From headA: nodesA + intersection + nodesAB + nodesB + intersection
// From headB: nodesB + intersection + nodesAB + nodesA + intersection
//
// Both nodes will travel: nodesA + nodesB + intersection + nodesAB nodes!
//
// Complexity: runtime O(n+m), space O(1).

class Solution {
public:
    ListNode* getIntersectionNode(ListNode* const headA,
                                  ListNode* const headB) const noexcept
    {
        auto nA = headA;
        for (auto nB = headB; nA != nB;) {
            nA = nA ? nA->next : headB;
            nB = nB ? nB->next : headA;
        }

        return nA;
    }
};

// Using a hash set.
// Complexity: runtime O(n+m), space O(n+m).

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

// Alternative Solution: Mutate the list to mark visits, then restore it after.
// Complexity: runtime O(n+m), space O(1).

class Solution {
public:
    ListNode* getIntersectionNode(ListNode* const headA,
                                  ListNode* const headB) const noexcept
    {
        ListNode* res = nullptr;
        for (auto nA = headA, nB = headB; nA || nB;) {
            if (nA) {
                if ((nA->val *= -1) > 0) {
                    res = nA;
                    break;
                }
                nA = nA->next;
            }

            if (nB) {
                if ((nB->val *= -1) > 0) {
                    res = nB;
                    break;
                }
                nB = nB->next;
            }
        }

        for (auto nA = headA, nB = headB; nA || nB != res;) {
            if (nA) {
                if (nA->val < 0)
                    nA->val *= -1;

                nA = nA->next;
            }

            if (nB != res) {
                nB->val *= -1;
                nB = nB->next;
            }
        }

        return res;
    }
};
