// https://leetcode.com/problems/remove-zero-sum-consecutive-nodes-from-linked-list
// Complexity: runtime O(n), space O(n).

class Solution {
public:
    ListNode *removeZeroSumSublists(ListNode *head)
    {
        ListNode beforeHead(0, head);
        unordered_map<int, ListNode *> memo;
        memo[0] = &beforeHead;
        int sum = 0;
        for (auto n = beforeHead.next; n; n = n->next) {
            sum += n->val;
            auto [it, inserted] = memo.emplace(sum, n);
            if (inserted) continue;

            int delsum = sum;
            for (auto m = it->second->next; m != n; m = m->next) {
                delsum += m->val;
                memo.erase(delsum);
            }
            it->second->next = n->next;
        }
        return beforeHead.next;
    }
};
