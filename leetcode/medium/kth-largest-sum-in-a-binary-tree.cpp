// https://leetcode.com/problems/kth-largest-sum-in-a-binary-tree
// Faster than 100%.
// Complexity: runtime O(n), space O(d).

class Solution {
    void dfs(const TreeNode *const t, vector<long long> &sums,
             const int d = 1) const
    {
        if (!t)
            return;
        if (sums.size() < d)
            sums.resize(d);
        sums[d - 1] += t->val;
        dfs(t->left, sums, d + 1);
        dfs(t->right, sums, d + 1);
    }

public:
    long long kthLargestLevelSum(const TreeNode *const root, const int k) const
    {
        vector<long long> sums;
        dfs(root, sums);
        if (sums.size() < k)
            return -1;
        const auto it = sums.begin() + (k - 1);
        nth_element(sums.begin(), it, sums.end(), greater{});
        return *it;
    }
};
