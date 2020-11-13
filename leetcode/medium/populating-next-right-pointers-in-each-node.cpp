// https://leetcode.com/problems/populating-next-right-pointers-in-each-node/
//
// Iterative, constant-space solution.
// Complexity: runtime O(n), space O(1).

/*
// Definition for a Node.
class Node {
public:
    int val;
    Node* left;
    Node* right;
    Node* next;

    Node() : val(0), left(NULL), right(NULL), next(NULL) {}

    Node(int _val) : val(_val), left(NULL), right(NULL), next(NULL) {}

    Node(int _val, Node* _left, Node* _right, Node* _next)
        : val(_val), left(_left), right(_right), next(_next) {}
};
*/

class Solution {
public:
    Node* connect(Node* const root) const noexcept
    {
        auto node = root;
        while (node && node->left) {
            const auto nextDepth = node->left; // left-most node
            while (node) {
                node->left->next = node->right;
                node->right->next = node->next ? node->next->left : nullptr;
                node = node->next;
            }
            node = nextDepth;
        }

        return root;
    }
};
