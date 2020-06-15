// https://leetcode.com/problems/search-in-a-binary-search-tree/
//
// Iterative solution.
// Complexity: runtime O(n) [unbalanced chain], space O(1).

/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
class Solution {
public:
    TreeNode* searchBST(TreeNode* root, const int val) const
    {
        while (root && val != root->val)
            root = val < root->val ? root->left : root->right;

        return root;
    }
};

// Alternative Solution: Recursive solution.
// Complexity: runtime O(n) [unbalanced chain], space O(n) [O(1) with tail
// recursion].

class Solution {
public:
    TreeNode* searchBST(TreeNode* root, const int val) const
    {
        // if you want to be naughty you can write everything as a single line
        // return statement using 2 ternaries
        if (!root || val == root->val)
            return root;

        return searchBST(val < root->val ? root->left : root->right, val);
    }
};
