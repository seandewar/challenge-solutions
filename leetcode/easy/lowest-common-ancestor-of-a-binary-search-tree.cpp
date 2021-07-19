// https://leetcode.com/problems/lowest-common-ancestor-of-a-binary-search-tree
//
// It's possible to do this using only constant extra-space.
//
// Complexity: runtime O(n), space O(n).

class Solution {
public:
    TreeNode* lowestCommonAncestor(TreeNode* const root, TreeNode* const p,
                                   TreeNode* const q) const
    {
        vector<TreeNode*> pAncestors;
        auto node = root;
        while (node != p) {
            pAncestors.push_back(node);
            node = p->val > node->val ? node->right : node->left;
        }
        pAncestors.push_back(node);
        node = root;
        auto it = cbegin(pAncestors);
        for (; it != cend(pAncestors) && node == *it; ++it) {
            if (node != q)
                node = q->val > node->val ? node->right : node->left;
        }
        return *(--it);
    }
};
