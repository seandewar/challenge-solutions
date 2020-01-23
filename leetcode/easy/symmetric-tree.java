// https://leetcode.com/problems/symmetric-tree/
//
// Complexity: runtime O(n), space O(n) [if tree consists of a single chain of
// length n].

/**
 * Definition for a binary tree node.
 * public class TreeNode {
 *     int val;
 *     TreeNode left;
 *     TreeNode right;
 *     TreeNode(int x) { val = x; }
 * }
 */
class Solution {
    private boolean isSymmetricImpl(final TreeNode left, final TreeNode right) {
        if (left == null || right == null)
            return left == null && right == null;

        return left.val == right.val
               && isSymmetricImpl(left.left, right.right)
               && isSymmetricImpl(left.right, right.left);
    }

    public boolean isSymmetric(final TreeNode root) {
        if (root == null)
            return true;

        return isSymmetricImpl(root.left, root.right);
    }
}

// Alternative Solution: Iterative solution.
// Complexity: runtime O(n), space O(n) [if tree consists of a single chain of
// length n].
/*
import java.util.Optional;

class Solution {
    public boolean isSymmetric(final TreeNode root) {
        if (root == null)
            return true;

        final var stack = new ArrayDeque<Optional<TreeNode>>();
        stack.add(Optional.ofNullable(root.left));
        stack.add(Optional.ofNullable(root.right));

        while (!stack.isEmpty()) {
            final var right = stack.pop();
            final var left = stack.pop();

            if (!left.isPresent() && !right.isPresent())
                continue;

            if (!left.isPresent() || !right.isPresent()
                || left.get().val != right.get().val)
                return false;

            stack.add(Optional.ofNullable(left.get().left));
            stack.add(Optional.ofNullable(right.get().right));
            stack.add(Optional.ofNullable(left.get().right));
            stack.add(Optional.ofNullable(right.get().left));
        }

        return true;
    }
}
*/
