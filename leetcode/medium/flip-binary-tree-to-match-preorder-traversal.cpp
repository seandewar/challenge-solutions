// https://leetcode.com/problems/flip-binary-tree-to-match-preorder-traversal
//
// Complexity: runtime O(n), space O(n) [O(logn) if exluding output and tree is
// balanced].

class Solution {
    size_t dfs(const TreeNode* const node, const vector<int>& voyage,
               vector<int>& out, size_t i = 0) const
    {
        if (!node)
            return i;
        if (i >= size(voyage) || node->val != voyage[i])
            return -1;
        if (const auto reti = dfs(node->right, voyage, out,
                                  dfs(node->left, voyage, out, ++i));
            reti != -1)
            return reti;

        out.push_back(voyage[i - 1]);
        return dfs(node->left, voyage, out, dfs(node->right, voyage, out, i));
    }

public:
    vector<int> flipMatchVoyage(const TreeNode* const root,
                                const vector<int>& voyage) const
    {
        vector<int> res;
        return dfs(root, voyage, res) == size(voyage) ? res : vector{-1};
    }
};
