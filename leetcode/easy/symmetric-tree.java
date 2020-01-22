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
    private boolean isSymmetricImpl(TreeNode left, TreeNode right) {
        if (left == null || right == null)
            return left == null && right == null;

        return left.val == right.val
               && isSymmetricImpl(left.left, right.right)
               && isSymmetricImpl(left.right, right.left);
    }

    public boolean isSymmetric(TreeNode root) {
        if (root == null)
            return true;

        return isSymmetricImpl(root.left, root.right);
    }
}

// Alternative Solution: Iterative solution.
// Complexity: runtime O(n), space O(n) [if tree consists of a single chain of
// length n].
/*
import java.util.Optional; // leetcode doesn't automatically import this

class Solution {
    public boolean isSymmetric(TreeNode root) {
        if (root == null)
            return true;

        // java doesn't allow the inserting of null in some containers such as
        // Deque (but sometimes does for Queue). this is because java is a meme
        // language that uses null as a return value for some member functions
        // to indicate an empty container (e.g Deque.poll()). we get around
        // this by using Optional, although we could also use a Queue that
        // allows null insertions (but java doesn't recommend inserting nulls
        // even if the container allows it anyway...)
        Deque<Optional<TreeNode>> stack = new ArrayDeque<>();

        stack.add(Optional.ofNullable(root.left));
        stack.add(Optional.ofNullable(root.right));

        while (!stack.isEmpty()) {
            Optional<TreeNode> right = stack.pop();
            Optional<TreeNode> left = stack.pop();

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
