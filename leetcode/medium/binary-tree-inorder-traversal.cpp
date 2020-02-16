// https://leetcode.com/problems/binary-tree-inorder-traversal/
//
// Recursion.
// Complexity: runtime O(n), space O(n) [O(logn) average].

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
    void inorderTraversal(const TreeNode* root, vector<int>& out) const
    {
        if (!root)
            return;

        inorderTraversal(root->left, out);
        out.push_back(root->val);
        inorderTraversal(root->right, out);
    }

public:
    vector<int> inorderTraversal(const TreeNode* root) const
    {
        vector<int> results;
        inorderTraversal(root, results);
        return results;
    }
};

// Alternative Solution: iteration + stack.
// Complexity: runtime O(n), space O(n) [O(logn) average].
/*
class Solution {
public:
    vector<int> inorderTraversal(const TreeNode* root) const
    {
        vector<int> results;

        // stack holds the nodes we've explored that we'll return to once we've
        // finished exploring their left subtrees. this allows us to come back
        // and push their values to the output in-order and explore their right
        // subtrees afterwards
        stack<reference_wrapper<const TreeNode>> pending;
        auto node = root;

        while (node || !empty(pending)) {
            if (node) {
                pending.push(*node);
                node = node->left;
                continue;
            }

            const auto top = pending.top();
            pending.pop();

            results.push_back(top.get().val);
            node = top.get().right;
        }

        return results;
    }
};
*/

// Alternative Solution: iteration + stack + std::variant.
// Complexity: runtime O(n), space O(n) [O(logn) average].
/*
class Solution {
public:
    vector<int> inorderTraversal(const TreeNode* root) const
    {
        vector<int> results;
        stack<variant<const TreeNode*, int>> s({root});

        while (!empty(s)) {
            const auto top = s.top();
            s.pop();

            if (top.index() == 0) {
                const auto node = get<0>(top);

                if (!node)
                    continue;

                s.push(node->right);
                s.push(node->val);
                s.push(node->left);
            } else if (top.index() == 1) {
                results.push_back(get<1>(top));
            }
        }

        return results;
    }
};
*/
