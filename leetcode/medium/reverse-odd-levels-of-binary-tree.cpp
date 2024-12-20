// https://leetcode.com/problems/reverse-odd-levels-of-binary-tree
// Complexity: runtime O(n), space O(n).

class Solution {
public:
    TreeNode *reverseOddLevels(TreeNode *const root) const
    {
        deque q{root};
        for (bool even = true; !q.empty(); even = !even) {
            const auto size = q.size();
            for (size_t i = 0; i < size; ++i) {
                const auto node = q.front();
                q.pop_front();
                if (node->left)
                    q.push_back(node->left);
                if (node->right)
                    q.push_back(node->right);
            }
            if (even) // next depth is odd
                for (size_t i = 0; i < q.size() / 2; ++i)
                    swap(q[i]->val, q[q.size() - 1 - i]->val);
        }
        return root;
    }
};
