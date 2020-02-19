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
    void inorderTraversal(const TreeNode* const root, vector<int>& out) const
    {
        if (!root)
            return;

        inorderTraversal(root->left, out);
        out.push_back(root->val);
        inorderTraversal(root->right, out);
    }

public:
    vector<int> inorderTraversal(const TreeNode* const root) const
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
    vector<int> inorderTraversal(const TreeNode* const root) const
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
    vector<int> inorderTraversal(const TreeNode* const root) const
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

// Alternative Solution: Morris traversal.
//
// This algorithm achieves O(1) extra space via "threading" of the tree, which
// ends up temporarily mutating the tree, adding temporary edges so that nodes
// can be returned to later to be visited after the full traversal of their
// children within their left sub-tree. In other words, this algorithm finds the
// in-order predecessor of a node and creates a temporary threading so that the
// node can be visited immediately afterwards (as a successor) as would be the
// case in the in-order traversal, doing so without a stack or other data
// structure.
//
// It's unlikely in an interview you'll ever be asked to implement this; it's a
// lot to remember (I personally never bother trying).
//
// Complexity: runtime O(n), space O(1).
/*
class Solution {
public:
    vector<int> inorderTraversal(TreeNode* const root) const
    {
        vector<int> results;
        auto node = root;

        while (node) {
            if (!node->left) {
                // no left sub-tree to explore; simply visit this node
                results.push_back(node->val);
                node = node->right;
            } else {
                // if a left child exists, find the node in this left sub-tree
                // where once fully traversed along with its children, would
                // result in our original node being visited immediately
                // afterwards when traversed in-order.
                //
                // in other words, we find the in-order predecessor of our node
                auto predecessor = node->left;

                // we may find that we have already discovered the predecessor
                // we're looking for; if this is true, we would have already set
                // its right child to our node, and thus, we should break early
                // and undo the threading
                while (predecessor->right && predecessor->right != node)
                    predecessor = predecessor->right;

                if (!predecessor->right) {
                    // predecessor found; make node its right child so it can be
                    // visited immediately after this as its successor
                    predecessor->right = node;
                    node = node->left;
                } else {
                    // predecessor was found previously and should already have
                    // the successor as its right child; visit the successor and
                    // undo the threading by removing the right edge
                    predecessor->right = nullptr;
                    results.push_back(node->val);
                    node = node->right;
                }
            }
        }

        return results;
    }
};
*/
