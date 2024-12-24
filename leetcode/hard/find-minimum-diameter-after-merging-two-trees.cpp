// https://leetcode.com/problems/find-minimum-diameter-after-merging-two-trees
// Complexity: runtime O(n+m), space O(n+m).

class Solution {
public:
    int minimumDiameterAfterMerge(const vector<vector<int>> &edges1,
                                  const vector<vector<int>> &edges2) const
    {
        const auto n = edges1.size() + 1, m = edges2.size() + 1;
        vector<unordered_set<int>> tree1(n), tree2(m);
        for (const auto &e : edges1) {
            tree1[e[0]].insert(e[1]);
            tree1[e[1]].insert(e[0]);
        }
        for (const auto &e : edges2) {
            tree2[e[0]].insert(e[1]);
            tree2[e[1]].insert(e[0]);
        }

        // Tree is undirected again, like a similar problem from a few days ago
        // -_-; pick a root (arbitrarily node 0), direct edges away from it, and
        // count the number of nodes on the longest path (which we get by adding
        // the 2 largest child depths of each node, returning the largest sum).
        const auto rootify_and_count_depth =
            [&](this const auto &self, auto &tree, const int node,
                unsigned &node_count_on_longest_path) -> unsigned {
            array<unsigned, 2> max_lens{};

            for (const auto child : tree[node]) {
                tree[child].erase(node);

                const auto it = min_element(max_lens.begin(), max_lens.end());
                *it = max(*it, self(tree, child, node_count_on_longest_path));
            }

            node_count_on_longest_path =
                max(node_count_on_longest_path, 1 + max_lens[0] + max_lens[1]);
            return 1 + *max_element(max_lens.begin(), max_lens.end());
        };

        unsigned node_count1 = 0, node_count2 = 0;
        rootify_and_count_depth(tree1, 0, node_count1);
        rootify_and_count_depth(tree2, 0, node_count2);

        return max(max(node_count1 - 1, node_count2 - 1),
                   1 + node_count1 / 2 + node_count2 / 2);
    }
};
