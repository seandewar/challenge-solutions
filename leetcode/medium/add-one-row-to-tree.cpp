// https://leetcode.com/problems/add-one-row-to-tree
//
// Complexity: runtime O(n), space O(n) [O(logn) if balanced].

/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left),
 * right(right) {}
 * };
 */
class Solution {
    void dfs(TreeNode* const node, const int v, const int d) const
    {
        if (!node)
            return;

        if (d == 1) {
            node->left = new TreeNode(v, node->left, nullptr);
            node->right = new TreeNode(v, nullptr, node->right);
            return;
        }

        dfs(node->left, v, d - 1);
        dfs(node->right, v, d - 1);
    }

public:
    TreeNode* addOneRow(TreeNode* const root, const int v, const int d) const
    {
        TreeNode fakeRoot(0, root, nullptr);
        dfs(&fakeRoot, v, d);
        return fakeRoot.left;
    }
};
