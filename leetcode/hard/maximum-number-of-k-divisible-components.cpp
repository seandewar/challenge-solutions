// https://leetcode.com/problems/maximum-number-of-k-divisible-components
// Complexity: runtime O(n), space O(n).

class Solution {
public:
    int maxKDivisibleComponents(int, const vector<vector<int>> &edges,
                                const vector<int> &values, const int k) const
    {
        vector<unordered_set<int>> tree(values.size());
        for (const auto &e : edges) {
            tree[e[0]].insert(e[1]);
            tree[e[1]].insert(e[0]);
        }

        // Tree is undirected (cringe), so select whatever node as the root,
        // then direct all edges away from it. Also calculate prefix sums
        // starting from the leaves to figure out the number of components.
        int ret = 0;
        const auto rootify_and_prefixify = [&](this const auto &self,
                                               const int node) -> long long {
            long long prefix = values[node];
            for (const int child : tree[node]) {
                tree[child].erase(node);
                prefix += self(child);
            }

            if (prefix % k == 0)
                ++ret;
            return prefix;
        };
        rootify_and_prefixify(0);

        return ret;
    }
};
