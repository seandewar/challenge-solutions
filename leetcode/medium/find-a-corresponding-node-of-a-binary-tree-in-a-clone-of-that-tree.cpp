// https://leetcode.com/problems/find-a-corresponding-node-of-a-binary-tree-in-a-clone-of-that-tree/
//
// Iterative solution.
// Complexity: runtime O(n), space O(n) [O(logn) if balanced tree].

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
    TreeNode* getTargetCopy(TreeNode* const original, TreeNode* const cloned,
                            const TreeNode* const target) const
    {
        stack s(deque{pair(original, cloned)});

        while (!empty(s)) {
            const auto [o, c] = s.top();
            s.pop();

            if (!o || !c)
                continue;

            if (o == target)
                return c;

            s.emplace(o->left, c->left);
            s.emplace(o->right, c->right);
        }

        return nullptr;
    }
};
