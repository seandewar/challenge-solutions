// https://leetcode.com/problems/convert-sorted-list-to-binary-search-tree
//
// Complexity: runtime O(n), space O(n).

class Solution {
    TreeNode* build(const vector<int>& vec, ptrdiff_t i, ptrdiff_t n) const
    {
        if (n <= 0)
            return nullptr;

        const size_t midIdx = i + n / 2;
        const auto left = build(vec, i, midIdx - i);
        const auto right = build(vec, midIdx + 1, n - midIdx + i - 1);
        return new TreeNode(vec[midIdx], left, right);
    }

public:
    TreeNode* sortedListToBST(const ListNode* head) const
    {
        vector<int> vec;
        for (; head; head = head->next)
            vec.push_back(head->val);

        return build(vec, 0, size(vec));
    }
};
