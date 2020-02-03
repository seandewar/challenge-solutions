// https://leetcode.com/problems/binary-tree-level-order-traversal/
//
// Recursion solution.
// Complexity: runtime O(n), space O(n) [stack overhead if tree is chain of
// length n; O(logn) average case].

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
    private void levelOrderImpl(final TreeNode root, final int depth,
            final List<List<Integer>> result) {
        if (root == null)
            return;

        // if there doesn't exist a 'row' in our results representing the
        // values at this depth yet, we will need to create one. because we
        // recurse depths 1 level at a time, the row we insert will always be
        // the correct one for this depth (which means no index OOB exception).
        // we could simply test if size() == depth instead, if we wanted to
        if (result.size() <= depth)
            result.add(new ArrayList<>());

        // in-order traversal of the tree will insert node values into the list
        // in level order. funnily enough, a pre-order traversal will also work
        levelOrderImpl(root.left, depth + 1, result);
        result.get(depth).add(root.val);
        levelOrderImpl(root.right, depth + 1, result);
    }

    public List<List<Integer>> levelOrder(TreeNode root) {
        final var result = new ArrayList<List<Integer>>();
        levelOrderImpl(root, 0, result);
        return result;
    }
}

// Alternative Solution: Iteration.
// Complexity: runtime O(n), space O(n) [stack overhead if tree is chain of
// length n; O(logn) average case].
/*
import java.util.Optional; // leetcode doesn't auto import this

class Solution {
    public List<List<Integer>> levelOrder(final TreeNode root) {
        final var results = new ArrayList<List<Integer>>();

        // a queue is used to allow for in-order traversal of nodes, however, a
        // pre-order traversal using a stack also produces the correct output
        // (just reverse the insertion order of the left and right children of
        // a node).
        //
        // NOTE: java Deques tend to disallow insertions of null elements, so
        // we use Optional instead (or you can just check for null root
        // prior/null left/right nodes in the loop explicitly)
        final var queue = new ArrayDeque<Optional<TreeNode>>();
        queue.add(Optional.ofNullable(root));

        for (int depth = 0; queue.size() > 0; ++depth) {
            final var size = queue.size();

            for (int i = 0; i < size; ++i) {
                final var front = queue.remove();

                if (!front.isPresent())
                    continue;

                if (results.size() <= depth)
                    results.add(new ArrayList<>());

                results.get(depth).add(front.orElseThrow().val);

                queue.add(Optional.ofNullable(front.orElseThrow().left));
                queue.add(Optional.ofNullable(front.orElseThrow().right));
            }
        }

        return results;
    }
}
*/
