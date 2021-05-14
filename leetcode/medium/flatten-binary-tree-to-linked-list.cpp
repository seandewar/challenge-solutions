// https://leetcode.com/problems/flatten-binary-tree-to-linked-list
//
// In-place.
// Complexity: runtime O(n), space O(n) [call stack].

class Solution {
    void dfs(TreeNode* node, TreeNode*& flatPrev) {
        if (node) {
            flatPrev = flatPrev->left = node;
            dfs(exchange(node->left, nullptr), flatPrev);
            dfs(exchange(node->right, nullptr), flatPrev);
        }
    }

public:
    void flatten(TreeNode* root) {
        TreeNode tempRoot(0, nullptr, root), *flatPrev = &tempRoot;
        dfs(root, flatPrev);
        for (auto node = tempRoot.left; node; node = node->right)
            node->right = exchange(node->left, nullptr);
    }
};
