// https://leetcode.com/problems/convert-sorted-array-to-binary-search-tree/
//
// Converts a sorted array of numbers to a balanced BST (each node's left &
// right subtrees' depths differ by no more than 1).
//
// Complexity: runtime O(n), space O(logn) [recursion overhead; output size
// (O(n)) is not considered in complexity calculations].

/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
class Solution {
    using NumsVec = vector<int>;
    using NumsVecSize = NumsVec::size_type;

    auto sortedArrayToBST(const NumsVec& nums,
                          const NumsVecSize beginIdx,
                          const NumsVecSize endIdx) const noexcept -> TreeNode*
    {
        // you may instead terminate if beginIdx >= endIdx, or also terminate
        // if beginIdx >= size(nums) || endIdx > size(nums) if you want to, but
        // this function will never be called with such arguments anyway
        if (beginIdx == endIdx)
            return {};

        // same as middleIdx = (beginIdx + endIdx) / 2, but this avoids overflow
        const auto middleIdx{beginIdx + (endIdx - beginIdx) / 2};
        const auto head{new TreeNode{nums[middleIdx]}};

        head->left = sortedArrayToBST(nums, beginIdx, middleIdx);
        head->right = sortedArrayToBST(nums, middleIdx + 1, endIdx);

        return head;
    }

public:
    auto sortedArrayToBST(const NumsVec& nums) const noexcept
    {
        return sortedArrayToBST(nums, 0, size(nums));
    }
};
