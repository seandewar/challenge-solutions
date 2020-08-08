// https://leetcode.com/problems/vertical-order-traversal-of-a-binary-tree/
//
// Complexity: runtime O(n), space O(n).

/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left),
 * right(right) {}
 * };
 */
class Solution {
    void buildValuesMatrix(const TreeNode* const root,
                           vector<map<ptrdiff_t, vector<int>>>& valMat,
                           ptrdiff_t& minCol, ptrdiff_t& maxCol,
                           const size_t depth = 0,
                           const ptrdiff_t col = 0) const
    {
        if (!root) {
            return;
        }

        minCol = min(minCol, col);
        maxCol = max(maxCol, col);

        valMat.resize(max(size(valMat), depth + 1));
        valMat[depth][col].push_back(root->val);

        buildValuesMatrix(root->left, valMat, minCol, maxCol, depth + 1,
                          col - 1);
        buildValuesMatrix(root->right, valMat, minCol, maxCol, depth + 1,
                          col + 1);
    }

public:
    vector<vector<int>> verticalTraversal(const TreeNode* const root) const
    {
        vector<map<ptrdiff_t, vector<int>>> valMat;

        auto minCol = numeric_limits<ptrdiff_t>::max();
        auto maxCol = numeric_limits<ptrdiff_t>::min();

        buildValuesMatrix(root, valMat, minCol, maxCol);

        vector<vector<int>> result(maxCol + 1 - minCol);

        for (auto& row : valMat) {
            for (auto& valsEntry : row) {
                auto& [col, vals] = valsEntry;
                sort(begin(vals), end(vals));

                auto& resultCol = result[col - minCol];
                resultCol.insert(end(resultCol), cbegin(vals), cend(vals));
            }
        }

        return result;
    }
};
