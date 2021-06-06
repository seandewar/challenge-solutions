// https://leetcode.com/problems/maximum-binary-tree
//
// Complexity: runtime O(n^2), space O(n).

class Solution {
    TreeNode* makeTree(const vector<int>& nums, const size_t l,
                       const size_t r) const
    {
        if (r - l == 0)
            return nullptr;

        const auto maxIt = max_element(begin(nums) + l, begin(nums) + r);
        const auto node = new TreeNode(*maxIt);
        node->left = makeTree(nums, l, maxIt - begin(nums));
        node->right = makeTree(nums, maxIt + 1 - begin(nums), r);
        return node;
    }

public:
    TreeNode* constructMaximumBinaryTree(const vector<int>& nums) const
    {
        return makeTree(nums, 0, size(nums));
    }
};
