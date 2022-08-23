// https://leetcode.com/problems/binary-tree-maximum-path-sum
// Complexity: runtime O(n), space O(n).

class Solution {
    int dfsPath(TreeNode *node, unordered_map<TreeNode *, pair<int, int>> &dp,
                bool root = false) const
    {
        if (!node) {
            return 0;
        }
        int l, r;
        if (auto it = dp.find(node); it != end(dp)) {
            l = it->second.first;
            r = it->second.second;
        } else {
            l = max(0, dfsPath(node->left, dp));
            r = max(0, dfsPath(node->right, dp));
            dp[node] = {l, r};
        }
        return node->val + max(root ? l + r : 0, max(l, r));
    }

    int dfs(TreeNode *node, unordered_map<TreeNode *, pair<int, int>> &dp) const
    {
        if (!node) {
            return numeric_limits<int>::min();
        }
        return max(dfsPath(node, dp, true),
                   max(dfs(node->left, dp), dfs(node->right, dp)));
    }

public:
    int maxPathSum(TreeNode *root) const
    {
        unordered_map<TreeNode *, pair<int, int>> dp;
        return dfs(root, dp);
    }
};
