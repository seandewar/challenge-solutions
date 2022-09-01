// https://leetcode.com/problems/count-good-nodes-in-binary-tree
// Complexity: runtime O(n), space O(n).

class Solution {
public:
    int goodNodes(const TreeNode *const root,
                  int maxv = numeric_limits<int>::min()) const
    {
        if (!root) {
            return 0;
        }
        maxv = max(maxv, root->val);
        return (root->val >= maxv) + goodNodes(root->left, maxv)
               + goodNodes(root->right, maxv);
    }
};
