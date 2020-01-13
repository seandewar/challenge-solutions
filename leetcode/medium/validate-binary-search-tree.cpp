// https://leetcode.com/problems/validate-binary-search-tree/
//
// Recursive solution that keeps track of the min and max values for a
// particular BST node and its subtree while recursing.
//
// Uses std::optional instead of INT_MIN/INT_MAX as the there exists a test
// case which specifically assigns INT_MIN and INT_MAX as a valid node value.
// If desired, the optionals can be replaced with pointers to the current
// min/max nodes instead, which is a more universal solution (e.g. the node's
// val type is some very large data type instead of just a simple int).
//
// Complexity: runtime O(n), space O(logn) if BST is balanced, otherwise O(n)
// [in the worst case, tree can consist of a chain of length n or similar].

/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
class Solution {
    bool validateBST(TreeNode* root,
                     optional<int> min = {}, optional<int> max = {})
    {
        if (!root)
            return true;

        if ((max && root->val >= *max) || (min && root->val <= *min))
            return false;

        return validateBST(root->left, min, root->val)
               && validateBST(root->right, root->val, max);
    }

public:
    bool isValidBST(TreeNode* root)
    {
        return validateBST(root);
    }
};
