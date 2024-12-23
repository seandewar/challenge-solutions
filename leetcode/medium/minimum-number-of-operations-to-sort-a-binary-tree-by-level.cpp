// https://leetcode.com/problems/minimum-number-of-operations-to-sort-a-binary-tree-by-level
// Complexity: runtime O(nlogn), space O(n).

class Solution {
public:
    int minimumOperations(const TreeNode *const root) const
    {
        int ret = 0;
        array<size_t, 100000> is;  // lookup arrays over hash tables/vectors has
        array<int, 100000> sorted; // this faster than 97%, but isn't required
        for (deque q{root}; !q.empty();) {
            const auto sz = q.size();
            for (size_t i = 0; i < sz; ++i) {
                const auto node = q[i];
                is[node->val - 1] = i;
                sorted[i] = node->val;
                if (node->left)
                    q.push_back(node->left);
                if (node->right)
                    q.push_back(node->right);
            }

            sort(sorted.begin(), sorted.begin() + sz);
            for (size_t i = 0; i < sz; ++i) {
                const auto val = q[i]->val, want_val = sorted[i];
                if (val != want_val) {
                    swap(q[i], q[is[want_val - 1]]);
                    swap(is[val - 1], is[want_val - 1]);
                    ++ret;
                }
            }
            q.erase(q.begin(), q.begin() + sz);
        }
        return ret;
    }
};
