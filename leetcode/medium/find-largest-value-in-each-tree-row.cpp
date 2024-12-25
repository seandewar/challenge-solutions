// https://leetcode.com/problems/find-largest-value-in-each-tree-row
// Complexity: runtime O(n), space O(d).

class Solution {
public:
    vector<int> largestValues(const TreeNode *const root) const
    {
        vector<int> ret;
        const auto dfs = [&](this const auto &self, const TreeNode *const node,
                             const size_t d) {
            if (!node)
                return;
            ret.resize(max(ret.size(), d + 1), numeric_limits<int>::min());
            ret[d] = max(ret[d], node->val);

            self(node->left, d + 1);
            self(node->right, d + 1);
        };

        dfs(root, 0);
        return ret;
    }
};
