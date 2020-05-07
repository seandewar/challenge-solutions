// https://leetcode.com/problems/cousins-in-binary-tree/
//
// Using a queue (breadth-first search).
// Complexity: runtime O(n), space O(n).

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
    bool isCousins(const TreeNode* const root, const int x, const int y) const
    {
        using ChildParentPair = pair<const TreeNode*, const TreeNode*>;

        queue<ChildParentPair> q({{root, nullptr}});
        const TreeNode* foundParent = nullptr;

        while (!empty(q)) {
            const auto depthSize = size(q);

            for (queue<ChildParentPair>::size_type i = 0; i < depthSize; ++i) {
                const auto [node, parent] = q.front();
                q.pop();

                if (!node)
                    continue;

                if (node->val == x || node->val == y) {
                    // because each node is guaranteed to have a unique val, we
                    // can assume that this node should be the cousin of the
                    // previous node that triggered the above condition if the
                    // condition was triggered again at this depth. also, the
                    // root node (parent == nullptr) cannot be a cousin of any
                    // other node
                    if (foundParent || !parent)
                        return parent && foundParent != parent;

                    foundParent = parent;
                }

                // no point checking the next depth if we're expecting the
                // cousin to be at this depth
                if (!foundParent) {
                    q.emplace(node->left, node);
                    q.emplace(node->right, node);
                }
            }

            // we found X or Y, but not the other at this depth; not cousins
            if (foundParent)
                return false;
        }

        return false; // X nor Y are in the tree
    }
};
