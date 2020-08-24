# https://leetcode.com/problems/sum-of-left-leaves/
#
# Complexity: runtime O(n), space O(n) [recursion overhead; O(logn) for balanced
# tree].

# Definition for a binary tree node.
# class TreeNode:
#     def __init__(self, val=0, left=None, right=None):
#         self.val = val
#         self.left = left
#         self.right = right
class Solution:
    def sumOfLeftLeaves(self, root: TreeNode, is_left: bool = False) -> int:
        if root is None:
            return 0

        if root.left is None and root.right is None and is_left:
            return root.val

        return self.sumOfLeftLeaves(root.left, True)     \
               + self.sumOfLeftLeaves(root.right, False)
