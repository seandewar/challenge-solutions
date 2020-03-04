// https://leetcode.com/problems/construct-binary-tree-from-preorder-and-inorder-traversal/
//
// Consider the order of pre-order and in-order traversals:
//
// Pre-order: Visit node, go left, go right (V, L, R).
// In-order:  Go left, visit node, go right (L, V, R).
//
// To reconstruct the original tree, we loop through the pre-order input,
// building nodes in the left subtree of the previous nodes in the input. The
// reason we do this is because in a pre-order traversal, nodes are visited
// first before their left and then right subtrees are traversed, causing the
// visit order to be topologically sorted from the root in the direction of each
// node's left subtree before their right (this has the initial effect of always
// visiting all of the nodes on the left-most side of the tree first). This is
// much easier to understand with a visual example:
//
//       1       Pre-order traversal: [1, 2, 4, 5, 3, 6]
//      / \      (In-order traversal: [4, 2, 5, 1, 3, 6])
//     2   3
//    / \   \
//   4   5   6
//
// However, the pre-order input itself isn't enough to build the tree, as we
// cannot tell which nodes are the right child of a parent:
//
//            1
//           /
//          2  Also produces the same pre-order traversal as the above example!
//         /   (In-order traversal differs, though: [6, 3, 5, 4, 2, 1])
//        4
//       /
//      5
//     /
//    3
//   /
//  6
//
// The in-order input can be used to determine this; the left subtree of a node
// is always traversed before the node is visited, essentially visiting nodes
// before their right subtrees are traversed. This means that seeing a node
// within the in-order input that we've previously seen in the pre-order input
// (and built into our output tree already) indicates that there are no more
// nodes in the pre-order input to build that are within that node's left
// subtree; they must instead be part of that node's right subtree until we
// eventually see a predecessor of that node within the in-order input, which
// means we've finished building all the children of that node in both subtrees.
//
// See code comments for more details - this was pretty hard to explain and I
// probably didn't do a great job. It's easy to understand once you draw it out,
// though.
//
// Complexity: runtime O(n) [n==nodes in tree], space O(n) [chain of n nodes
// that are all the left child of their parent].

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
    public TreeNode buildTree(final int[] preorder, final int[] inorder) {
        // map of nodes to return to once we've finished building their left
        // subtrees so that we can start building their right subtrees.
        //
        // because the inputs only contain node values, this mapping of node
        // values to nodes allows us to quickly navigate to the specified node
        // in our output tree (leetcode question guarantees no duplicate values)
        final var rightSubtreesTodo = new HashMap<Integer, TreeNode>();
        TreeNode head = null, curr = null;

        for (int preIdx = 0, inIdx = 0; preIdx < preorder.length; ++preIdx) {
            var insertRight = false;

            // if we have already inserted the node at inorder[inIdx] into our
            // tree (we have already processed it from preorder[]), then there
            // are no more children left to insert into its left subtree, so we
            // should instead navigate back to it and start inserting children
            // into its right subtree
            while (rightSubtreesTodo.containsKey(inorder[inIdx])) {
                curr = rightSubtreesTodo.remove(inorder[inIdx++]);
                insertRight = true;
            }

            // remember to add this new node to our map of nodes to come back to
            // later once we've finished building its left subtree
            final var newNode = new TreeNode(preorder[preIdx]);
            rightSubtreesTodo.put(newNode.val, newNode);

            if (head == null)
                curr = head = newNode;
            else if (insertRight)
                curr = curr.right = newNode;
            else
                curr = curr.left = newNode;
        }

        return head;
    }
}

// Alternative Solution: rightSubtreesTodo as a stack instead of a map.
//
// The nodes to return to in order to build their right subtrees (if any) will
// always be the last few nodes inserted into the rightSubtreesTodo map. This
// relates to the fact that both the pre-order and in-order traversals can be
// work recursively. This entire solution can also be implemented recursively,
// too.
//
// Complexity: runtime O(n) [n==nodes in tree], space O(n) [chain of n nodes
// that are all the left child of their parent].
/*
class Solution {
    public TreeNode buildTree(final int[] preorder, final int[] inorder) {
        final var rightSubtreesTodo = new ArrayDeque<TreeNode>();
        TreeNode head = null, curr = null;

        for (int preIdx = 0, inIdx = 0; preIdx < preorder.length; ++preIdx) {
            var insertRight = false;

            while (rightSubtreesTodo.size() > 0
                   && rightSubtreesTodo.peek().val == inorder[inIdx]) {
                curr = rightSubtreesTodo.pop();
                ++inIdx;
                insertRight = true;
            }

            final var newNode = new TreeNode(preorder[preIdx]);
            rightSubtreesTodo.push(newNode);

            if (head == null)
                curr = head = newNode;
            else if (insertRight)
                curr = curr.right = newNode;
            else
                curr = curr.left = newNode;
        }

        return head;
    }
}
*/
