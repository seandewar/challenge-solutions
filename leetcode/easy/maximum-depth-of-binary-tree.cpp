// https://leetcode.com/problems/maximum-depth-of-binary-tree/
//
// Complexity: runtime O(n), space O(logn) if BST balanced, O(n) otherwise
// [recursion overhead].

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
public:
    int maxDepth(TreeNode* root)
    {
        return root ? 1 + max(maxDepth(root->left), maxDepth(root->right)) : 0;
    }
};
