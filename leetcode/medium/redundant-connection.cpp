// https://leetcode.com/problems/redundant-connection
// Complexity: runtime O(E), space O(E) [where E = max number of edges].

class Solution {
public:
    vector<int> findRedundantConnection(const vector<vector<int>> &edges) const
    {
        array<vector<int>, 1000> graph; // stores edge indices, not nodes
        for (int i = 0; i < edges.size(); ++i) {
            graph[edges[i][0] - 1].push_back(i);
            graph[edges[i][1] - 1].push_back(i);
        }

        array<bool, 1000> visited_nodes{}, visited_edges{};
        vector<int> path_edge_is{};
        const auto find_cycle = [&](this const auto &self,
                                    const int node_i = 0) {
            if (exchange(visited_nodes[node_i], true))
                return node_i + 1;

            for (const int edge_i : graph[node_i]) {
                if (exchange(visited_edges[edge_i], true))
                    continue;

                path_edge_is.push_back(edge_i);

                const auto &edge = edges[edge_i];
                const int other_node = edge[edge[0] - 1 == node_i],
                          cycle_node = self(other_node - 1);
                if (cycle_node > 0)
                    return cycle_node;

                path_edge_is.pop_back(); // back-track
            }

            return 0;
        };

        const int cycle_node = find_cycle();
        const auto end_it = find_if(
            path_edge_is.rbegin() + 1, path_edge_is.rend(), [&](const int i) {
                return edges[i][0] == cycle_node || edges[i][1] == cycle_node;
            });

        return edges[*max_element(path_edge_is.rbegin(), end_it)];
    }
};
