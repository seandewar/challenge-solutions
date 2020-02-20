// https://leetcode.com/problems/serialize-and-deserialize-binary-tree/
//
// Complexity: (de-)serialization: runtime O(n), space O(n).

import java.util.Optional;

/**
 * Definition for a binary tree node.
 * public class TreeNode {
 *     int val;
 *     TreeNode left;
 *     TreeNode right;
 *     TreeNode(int x) { val = x; }
 * }
 */
public class Codec {
    // Encodes a tree to a single string.
    public String serialize(final TreeNode root) {
        // i've chosen to serialize in breadth-first order; you can choose any
        // order you wish (as long as you de-serialize in the same order)
        final var q = new ArrayDeque<Optional<TreeNode>>();
        q.add(Optional.ofNullable(root));

        // nullStreak is used to trim a chain of null nodes from the end of the
        // output (if any), as they're unnecessary (you don't need to implement
        // this optimization if you don't want to)
        int nullStreak = 0;
        final var sb = new StringBuilder();

        while (q.size() > 0) {
            final var front = q.poll();

            if (front.isPresent()) { // non-null node
                final var node = front.orElseThrow();

                // append the serialization for the chain of null nodes we came
                // across before this node (if any). this will not happen if the
                // serialization would contain a chain of null nodes at the end,
                // which ends up trimming unnecessary nulls from the output
                for (int i = 0; i < nullStreak; ++i)
                    sb.append(',');

                nullStreak = 0;
                sb.append(node.val);
                sb.append(',');

                q.add(Optional.ofNullable(node.left));
                q.add(Optional.ofNullable(node.right));
            } else { // null node
                ++nullStreak;
            }
        }

        return sb.toString();
    }

    // Decodes your encoded data to a tree.
    public TreeNode deserialize(final String data) {
        final var vals = data.split(",");

        TreeNode root = null;
        final var q = new ArrayDeque<TreeNode>();

        for (int i = 0; i < vals.length; ++i) {
            if (!vals[i].equals("")) { // non-null node
                final var newNode = new TreeNode(Integer.valueOf(vals[i]));
                q.add(newNode);

                if (i == 0) // root node
                    root = newNode;
                else if ((i - 1) % 2 == 0) // left child of node at queue front
                    q.peek().left = newNode;
                else // right child of node at queue front
                    q.peek().right = newNode;
            }

            // if we processed the right child (even if it's null), then there's
            // no more children to process for the node at the front of the
            // queue; remove it from the queue
            if (i > 0 && (i - 1) % 2 == 1)
                q.remove();
        }

        return root;
    }
}

// Your Codec object will be instantiated and called as such:
// Codec codec = new Codec();
// codec.deserialize(codec.serialize(root));
