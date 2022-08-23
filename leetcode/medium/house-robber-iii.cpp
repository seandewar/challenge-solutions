// https://leetcode.com/problems/house-robber-iii
// Complexity: runtime O(nlogn) [O(n) is possible using an unordered_map with a
// custom hash, but I'm lazy], space O(n).

class Solution {
    int dfs(const TreeNode *const node, const bool rob,
            map<pair<const TreeNode *, bool>, int> &dp) const
    {
        if (!node) {
            return 0;
        }
        if (const auto it = dp.find({node, rob}); it != end(dp)) {
            return it->second;
        }
        int l = dfs(node->left, false, dp), r = dfs(node->right, false, dp);
        if (!rob) {
            l = max(l, dfs(node->left, true, dp));
            r = max(r, dfs(node->right, true, dp));
        }
        const int ret = l + r + (rob ? node->val : 0);
        dp[{node, rob}] = ret;
        return ret;
    }

public:
    int rob(const TreeNode *const root) const
    {
        map<pair<const TreeNode *, bool>, int> dp;
        return max(dfs(root, true, dp), dfs(root, false, dp));
    }
};
