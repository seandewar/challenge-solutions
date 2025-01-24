// https://leetcode.com/problems/find-eventual-safe-states
// Complexity: runtime O(n+m), space O(n).

class Solution {
public:
    vector<int> eventualSafeNodes(const vector<vector<int>> &graph) const
    {
        enum class Safety {
            Unsafe,
            Safe,
            Dunno,
            LemmeSee,
        };
        vector safe(graph.size(), Safety::Dunno);

        const auto is_safe = [&](this const auto &self,
                                 const int node) -> bool {
            if (safe[node] == Safety::Safe || safe[node] == Safety::Unsafe)
                return safe[node] == Safety::Safe;

            if (exchange(safe[node], Safety::LemmeSee) == Safety::LemmeSee) {
                safe[node] = Safety::Unsafe;
                return false;
            }

            bool are_we_safe = true;
            for (const int other : graph[node]) {
                if (!self(other)) {
                    // if other isn't safe, then we and any earlier nodes in
                    // this path aren't safe either; we can stop checking now,
                    // but we instead still continue checking if other nodes
                    // remain safe...
                    are_we_safe = false;
                }
            }

            safe[node] = are_we_safe ? Safety::Safe : Safety::Unsafe;
            return are_we_safe;
        };

        vector<int> ret;
        for (int node = 0; node < graph.size(); ++node)
            if (is_safe(node))
                ret.push_back(node);

        return ret;
    }
};
