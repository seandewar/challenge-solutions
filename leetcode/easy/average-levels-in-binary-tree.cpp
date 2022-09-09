// https://leetcode.com/problems/average-levels-in-binary-tree
// Complexity: runtime O(n), space O(n).

class Solution {
public:
    vector<double> averageOfLevels(TreeNode *root)
    {
        vector<double> ret;
        queue<TreeNode *> q{{root}};
        while (!empty(q)) {
            long long tot = 0;
            int num = 0;
            for (int sz = size(q); sz > 0; --sz) {
                auto node = q.front();
                q.pop();
                if (node->left) {
                    q.push(node->left);
                }
                if (node->right) {
                    q.push(node->right);
                }
                tot += node->val;
                ++num;
            }
            ret.push_back(static_cast<double>(tot) / num);
        }
        return ret;
    }
};
