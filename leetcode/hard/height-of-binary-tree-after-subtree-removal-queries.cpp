// https://leetcode.com/problems/height-of-binary-tree-after-subtree-removal-queries
// I have created a monstrosity.
// Complexity: runtime O(nlogn), space O(n).

class Solution {
    void getNodeDepths(const TreeNode *const t, vector<pair<int, int>> &node_ds,
                       const int d = 0) const
    {
        if (!t)
            return;
        node_ds.emplace_back(d, t->val);
        getNodeDepths(t->left, node_ds, d + 1);
        getNodeDepths(t->right, node_ds, d + 1);
    }

    int getMaxHeights(const TreeNode *const t, unordered_map<int, int> &max_hs,
                      priority_queue<pair<int, int>> &d_heap,
                      priority_queue<pair<int, int>> &to_pop, const int d = 0,
                      const int lsib_bestd = -1) const
    {
        if (!t)
            return -1;
        const int l_maxd = getMaxHeights(t->left, max_hs, d_heap, to_pop, d + 1,
                                         lsib_bestd),
                  r_maxd = getMaxHeights(t->right, max_hs, d_heap, to_pop,
                                         d + 1, max(lsib_bestd, l_maxd));
        to_pop.emplace(d, t->val);
        while (!to_pop.empty() && to_pop.top().second == d_heap.top().second) {
            to_pop.pop();
            d_heap.pop();
        }
        max_hs[t->val] =
            max(lsib_bestd, d_heap.empty() ? 0 : d_heap.top().first);
        return max(max(d, l_maxd), r_maxd);
    }

public:
    vector<int> treeQueries(const TreeNode *const r,
                            const vector<int> &queries) const
    {
        vector<pair<int, int>> node_ds;
        getNodeDepths(r, node_ds);
        priority_queue<pair<int, int>> d_heap(less<pair<int, int>>{},
                                              std::move(node_ds)),
            to_pop;

        unordered_map<int, int> max_hs;
        max_hs.reserve(d_heap.size());
        getMaxHeights(r, max_hs, d_heap, to_pop);

        vector<int> ret;
        ret.reserve(queries.size());
        for (const int q : queries)
            ret.push_back(max_hs.at(q));
        return ret;
    }
};
