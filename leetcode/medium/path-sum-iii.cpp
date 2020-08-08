// https://leetcode.com/problems/path-sum-iii/
//
// Complexity: runtime O(n^2) [O(n*logn) for balanced tree], space O(n)
// [recursion overhead; O(logn) for a balanced tree]

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
public:
    template<bool IsSubTreePath = true>
    int pathSum(const TreeNode* const root, const int targetSum,
                int currSum = 0) const noexcept
    {
        if (!root) {
            return 0;
        }

        currSum += root->val;

        int result = (currSum == targetSum ? 1 : 0)
                     + pathSum<IsSubTreePath>(root->left, targetSum, currSum)
                     + pathSum<IsSubTreePath>(root->right, targetSum, currSum);

        if constexpr (IsSubTreePath) {
            result += pathSum<false>(root->left, targetSum)
                      + pathSum<false>(root->right, targetSum);
        }

        return result;
    }
};
