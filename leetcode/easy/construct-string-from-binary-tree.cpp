// https://leetcode.com/problems/construct-string-from-binary-tree
// Quick 'n ugly code.
// Complexity: runtime O(n), space O(n).

class Solution {
    void dfs(TreeNode *node, string &s, bool right = false, bool root = false)
    {
        if (!node) {
            if (right) {
                s += "()";
            }
            return;
        }
        if (!root) {
            s.push_back('(');
        }
        s += to_string(node->val);
        dfs(node->left, s, node->right);
        dfs(node->right, s);
        if (!root) {
            s.push_back(')');
        }
    }

public:
    string tree2str(TreeNode *root)
    {
        string ret;
        dfs(root, ret, false, true);
        return ret;
    }
};
