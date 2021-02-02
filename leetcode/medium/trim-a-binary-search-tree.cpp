// https://leetcode.com/problems/trim-a-binary-search-tree
//
// Complexity: runtime O(n), space O(n); O(logn) if balanced.

/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), low(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), low(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), low(left),
 * right(right) {}
 * };
 */
class Solution {
public:
    TreeNode* trimBST(TreeNode* const root, const int low,
                      const int high) const noexcept
    {
        if (!root)
            return nullptr;

        if (root->val > high)
            return trimBST(root->left, low, high);
        if (root->val < low)
            return trimBST(root->right, low, high);

        root->left = trimBST(root->left, low, high);
        root->right = trimBST(root->right, low, high);
        return root;
    }
};
