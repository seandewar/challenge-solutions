// https://leetcode.com/problems/count-complete-tree-nodes
// Complexity: runtime [less than] O(n), space O(logn) [BST is complete].

class Solution {
    int findDepth(const TreeNode *const root) const noexcept
    {
        return root ? 1 + findDepth(root->left) : 0;
    }

    pair<int, bool> dfs(const TreeNode *const root, const int maxDepth,
                        const int depth) const noexcept
    {
        if (depth < maxDepth) {
            const auto [lCount, lEnd] = dfs(root->left, maxDepth, depth + 1);
            if (lEnd) {
                return {lCount, true};
            }
            const auto [rCount, rEnd] = dfs(root->right, maxDepth, depth + 1);
            return {lCount + rCount, rEnd};
        }
        return {!!root, !root};
    }

public:
    int countNodes(const TreeNode *const root) const noexcept
    {
        const int depth = findDepth(root);
        const auto [count, _] = dfs(root, depth, 1);
        return (depth > 1 ? (1 << (depth - 1)) - 1 : 0) + count;
    }
};
