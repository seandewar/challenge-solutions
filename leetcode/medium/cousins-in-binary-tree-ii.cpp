// https://leetcode.com/problems/cousins-in-binary-tree-ii
// Complexity: runtime O(n), space O(d).

class Solution {
    void getSums(const TreeNode *const t, vector<int> &sums,
                 const int d = 1) const
    {
        if (!t)
            return;
        if (d > sums.size())
            sums.resize(d);

        sums[d - 1] += t->val;
        getSums(t->left, sums, d + 1);
        getSums(t->right, sums, d + 1);
    }

    void replaceValues(TreeNode *const t, const vector<int> &sums,
                       const int d = 1) const
    {
        if (t && d == 1)
            t->val = 0;
        if (!t || d >= sums.size())
            return;

        replaceValues(t->left, sums, d + 1);
        replaceValues(t->right, sums, d + 1);
        const int csum = sums[d] - (t->left ? t->left->val : 0)
                         - (t->right ? t->right->val : 0);
        if (t->left)
            t->left->val = csum;
        if (t->right)
            t->right->val = csum;
    }

public:
    TreeNode *replaceValueInTree(TreeNode *const r) const
    {
        vector<int> sums;
        getSums(r, sums);
        replaceValues(r, sums);
        return r;
    }
};
