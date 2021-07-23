// https://leetcode.com/problems/binary-tree-pruning
//
// Complexity: runtime O(n), space O(n) [O(logn) if balanced].

class Solution {
public:
    TreeNode* pruneTree(TreeNode* const root) const noexcept
    {
        if (!root)
            return nullptr;

        root->left = pruneTree(root->left);
        root->right = pruneTree(root->right);

        return root->left || root->right || root->val == 1 ? root : nullptr;
    }
};
