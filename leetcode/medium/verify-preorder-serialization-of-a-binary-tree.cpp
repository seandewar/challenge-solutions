// https://leetcode.com/problems/verify-preorder-serialization-of-a-binary-tree/
//
// Verfication via counting how many children are expected from each node in
// the tree, and how many nodes are actually present:
//
// We start by expecting to see the root node (null or not-null) first.
// Then, for each node in the tree, ensure we expected to see this node, then
// if this node is a:
//   > Non-null node: Expect to see 2 children. (1 net child)
//   > Null node: Expect to see 0 children. (-1 net children)
//
// Complexity: runtime O(n), space O(1).

class Solution {
public:
    bool isValidSerialization(string preorder)
    {
        if (empty(preorder))
            return true;

        // expect a root node (or null root node)
        string::size_type expectedNodes = 1;

        for (string::size_type i = 0; i < size(preorder);) {
            // expect node (or null node) is a child of another. if we're not
            // expecting any nodes, then the input has too many nodes
            if (expectedNodes == 0)
                return false;

            // not assuming input node values are 1 character long; question
            // doesn't explicitly state this
            const auto iNext = min(preorder.find_first_of(',', i),
                                   size(preorder));

            // expect 1 net child for each non-null node, -1 for each null node
            expectedNodes += preorder[i] != '#' || iNext - i != 1 ? 1 : -1;

            i = iNext + 1; // +1 skips comma separator
        }

        return expectedNodes == 0; // shouldn't be expecting more nodes if eof
    }
};

// The net-checking behaviour of this loop is equivalent to:
//
// for (string::size_type i = 0; i < size(preorder);) {
//     if (expectedNodes-- == 0) // must have expected this node (-1)
//         return false;
//
//     const auto iNext = min(preorder.find_first_of(',', i),
//             size(preorder));
//
//     if (preorder[i] != '#' || iNext - i != 1)
//         expectedNodes += 2; // expect 2 children for each non-null node (+2)
//
//     i = iNext + 1;
// }

// Alternative Solution: verification via pre-order Depth-First Search (without
// reconstructing the tree)
//
// Complexity: runtime O(n), space O(n). [space due to preMap pre-process - can
// be disregarded as just O(logn) from the DFS recursion stack if we don't
// assume that node input values can be larger than 1 char in length]

/*
class Solution {
    bool validatePreDfs(const vector<bool>& preMap, vector<bool>::size_type& i)
    {
        if (i >= size(preMap))
            return false;
        if (!preMap[i])
            return true;

        return validatePreDfs(preMap, ++i) && validatePreDfs(preMap, ++i);
    }

public:
    bool isValidSerialization(string preorder)
    {
        // node values unneeded - this could instead be replaced by an
        // unordered_set of null node indices & a variable storing the number
        // of nodes (& null nodes) in the input - size still O(n) (e.g if we
        // have an input of all null nodes)
        vector<bool> preMap;
        for (string::size_type i = 0; i < size(preorder);) {
            // not assuming input node values are 1 character long; question
            // doesn't explicitly state this
            const auto iNext = min(preorder.find_first_of(',', i),
                                   size(preorder));
            preMap.push_back(preorder[i] != '#' || iNext - i != 1);
            i = iNext + 1; // +1 skips comma separator
        }

        // check that the tree is valid when traversed pre-order and that we
        // actually ended up traversing exactly all nodes (& null nodes) in the
        // input
        vector<bool>::size_type i = 0;
        return validatePreDfs(preMap, i) && i == size(preMap) - 1;
    }
};
*/
