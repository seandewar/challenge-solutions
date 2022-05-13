// https://leetcode.com/problems/populating-next-right-pointers-in-each-node-ii
// Queue solution.
// Complexity: runtime O(n), space O(n).

class Solution {
public:
    Node *connect(Node *const root) const
    {
        queue<Node *> q;
        if (!root)
            return nullptr;

        while (!empty(q)) {
            const auto sz = size(q);
            for (queue<Node *>::size_type i = 0; i < sz; ++i) {
                const auto n = q.front();
                q.pop();

                if (i < sz - 1)
                    n->next = q.front();
                if (n->left)
                    q.push(n->left);
                if (n->right)
                    q.push(n->right);
            }
        }
        return root;
    }
};
