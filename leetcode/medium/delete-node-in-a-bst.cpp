// https://leetcode.com/problems/delete-node-in-a-bst/
//
// Complexity: runtime O(logn), space O(logn) [if balanced, O(n) otherwise for
// both (specifically, O(height of tree))].
//
// It's possible to get rid of the recursive call in the case of 2 children,
// which will make this O(1) space.

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
public:
    TreeNode* deleteNode(TreeNode* root, const int key) const noexcept
    {
        // find the node
        TreeNode *node = root, **prevBranch = &root;

        while (node && node->val != key) {
            prevBranch = key > node->val ? &node->right : &node->left;
            node = *prevBranch;
        }

        if (!node) {
            return root;
        }

        // delete the node
        if (node->left && node->right) {
            // case 1: node has 2 children; find the largest node in the child
            //         subtree that is smaller than this node, replace this node
            //         with its value, and recursively delete that child node
            auto other = node->left;

            while (other->right) {
                other = other->right;
            }

            node->val = other->val;
            node->left = deleteNode(node->left, other->val);
        } else {
            // case 2: node has no children or 1 child; replace node with that
            //         child if it exists, otherwise null it
            *prevBranch = node->left ? node->left : node->right;
            delete node;
        }

        return root;
    }
};
