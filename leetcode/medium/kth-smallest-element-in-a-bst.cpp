// https://leetcode.com/problems/kth-smallest-element-in-a-bst
// Complexity: runtime O(n), space O(logn) if balanced, O(n) otherwise.

class Solution {
    int dfs(const TreeNode *const node, int &k) const noexcept
    {
        if (!node)
            return -1;
        if (const int res = dfs(node->left, k); res != -1)
            return res;
        if (--k == 0)
            return node->val;

        return dfs(node->right, k);
    }

public:
    int kthSmallest(const TreeNode *const root, int k) const noexcept
    {
        return dfs(root, k);
    }
};
