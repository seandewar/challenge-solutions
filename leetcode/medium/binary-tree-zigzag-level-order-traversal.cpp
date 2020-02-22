// https://leetcode.com/problems/binary-tree-zigzag-level-order-traversal/
//
// Using std::reverse to zig-zag.
// Complexity: runtime O(n), space O(n).

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
    vector<vector<int>> zigzagLevelOrder(const TreeNode* const root) const
    {
        vector<vector<int>> results;
        queue<const TreeNode*> q({root});

        while (!empty(q)) {
            vector<int> level;

            for (auto remaining = size(q); remaining > 0; --remaining) {
                const auto front = q.front();
                q.pop();

                if (!front)
                    continue;

                level.push_back(front->val);
                q.push(front->left);
                q.push(front->right);
            }

            if (empty(level))
                continue;

            if (size(results) % 2 == 1)
                reverse(begin(level), end(level));

            results.push_back(move(level));
        }

        return results;
    }
};

// Alternative Solution: using std::deque to zig-zag.
//
// This requires a bit more thinking than what you may initially expect, as you
// have to modify _both_ your popping and pushing order to achieve the zig-zag
// effect. See code comments for details.
//
// Complexity: runtime O(n), space O(n).
/*
class Solution {
public:
    vector<vector<int>> zigzagLevelOrder(const TreeNode* const root) const
    {
        vector<vector<int>> results;
        deque<const TreeNode*> q({root});

        while (!empty(q)) {
            const bool reverse = size(results) % 2 == 1;
            vector<int> level;

            for (auto remaining = size(q); remaining > 0; --remaining) {
                // pop the front if not zig-zagging, otherwise pop from the back
                const auto front = reverse ? q.back() : q.front();

                if (reverse)
                    q.pop_back();
                else
                    q.pop_front();

                if (!front)
                    continue;

                level.push_back(front->val);

                // to achieve the zig-zagging, we also need to consider where we
                // push the children into the deque and in what order we do so.
                // if we're zig-zagging, we will be visiting the nodes at this
                // level in right-to-left order, so we have to push the children
                // we visit in such a way where we can visit them in
                // left-to-right order for the next level
                if (reverse) {
                    q.push_front(front->right); // *right* to the *front* first
                    q.push_front(front->left);
                } else {
                    q.push_back(front->left); // *left* to the *back* first
                    q.push_back(front->right);
                }
            }

            if (!empty(level))
                results.push_back(move(level));
        }

        return results;
    }
};
*/
