// https://leetcode.com/problems/merge-k-sorted-lists
//
// Min-heap solution.
//
// This solution is O(1) space due to moving the non-const "lists" argument.
//
// However, although it's mutable, we cannot construct the resulting list
// in-place by re-ordering the "next" pointers in the input lists, as the
// LeetCode problem runner code will cause a heap-use-after-free; it's still
// O(1) space however, as output is ignored in these calculations.
//
// Complexity: runtime O(n*logm), space O(1). [where n = num nodes in all lists,
//                                                   m = num lists]

class Solution {
public:
    ListNode* mergeKLists(vector<ListNode*>& lists) const
    {
        const auto cmp = [](const auto a, const auto b) noexcept {
            return !a || !b ? !!a : a->val > b->val; // may have some NULL lists
        };
        priority_queue<ListNode*, vector<ListNode*>, decltype(cmp)> heap(
            cmp, move(lists));

        ListNode fakeHead;
        auto node = &fakeHead;
        while (!empty(heap)) {
            const auto top = heap.top();
            heap.pop();

            if (top) {
                node = node->next = new ListNode(top->val);
                heap.push(top->next);
            }
        }

        return fakeHead.next;
    }
};

// Alternative Solution: Min-heap of all node values from all lists.
//
// Slower than the above solution, as both the insertion time and space
// complexity is now proportional to the total number of nodes in ALL lists.
//
// Complexity: runtime O(n*logn), space O(n). [where n = num nodes in all lists]

class Solution {
public:
    ListNode* mergeKLists(const vector<ListNode*>& lists) const
    {
        vector<int> vals;
        for (auto n : lists) {
            for (; n; n = n->next)
                vals.push_back(n->val);
        }

        priority_queue<int, vector<int>, greater<int>> heap(greater<int>(),
                                                            move(vals));
        ListNode fakeHead;
        auto node = &fakeHead;
        while (!empty(heap)) {
            const auto top = heap.top();
            heap.pop();
            node = node->next = new ListNode(top);
        }

        return fakeHead.next;
    }
};
