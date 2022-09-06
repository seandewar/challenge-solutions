// https://leetcode.com/problems/node-ary-tree-level-order-traversal
// Complexity: runtime O(n), space O(n).

class Solution {
public:
    vector<vector<int>> levelOrder(Node *root) const
    {
        vector<vector<int>> ret;
        if (!root) {
            return ret;
        }
        queue<Node *> q{{root}};
        while (!empty(q)) {
            vector<int> level;
            for (int sz = size(q); sz > 0; --sz) {
                auto node = q.front();
                q.pop();
                level.push_back(node->val);
                for (auto child : node->children) {
                    q.push(child);
                }
            }
            ret.push_back(move(level));
        }
        return ret;
    }
};
