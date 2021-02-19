// https://leetcode.com/problems/convert-bst-to-greater-tree
//
// Complexity: runtime O(n), space O(n) [O(logn) if balanced BST].

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
    auto dfs(TreeNode* const root, const int extraSum = 0) const noexcept -> int
    {
        if (!root)
            return extraSum;

        const int rightSum = dfs(root->right, extraSum);
        root->val += rightSum;
        return dfs(root->left, root->val);
    }

public:
    auto convertBST(TreeNode* const root) const noexcept -> TreeNode*
    {
        dfs(root);
        return root;
    }
};
