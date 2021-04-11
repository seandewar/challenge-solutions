// https://leetcode.com/problems/deepest-leaves-sum
//
// Complexity: runtime O(n), space O(n) if unbalanced; O(logn) otherwise.

class Solution {
    void dfs(const TreeNode* const root, const int depth, int& maxDepth,
             int& sum) const
    {
        if (!root)
            return;

        if (depth > maxDepth) {
            maxDepth = depth;
            sum = 0;
        }
        if (depth == maxDepth)
            sum += root->val;

        dfs(root->left, depth + 1, maxDepth, sum);
        dfs(root->right, depth + 1, maxDepth, sum);
    }

public:
    int deepestLeavesSum(const TreeNode* const root) const
    {
        int maxDepth = 0, sum = 0;
        dfs(root, 0, maxDepth, sum);
        return sum;
    }
};
