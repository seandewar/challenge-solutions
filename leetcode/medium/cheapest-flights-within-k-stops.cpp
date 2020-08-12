// https://leetcode.com/problems/cheapest-flights-within-k-stops/
//
// Shortest Path Faster Algorithm (SPFA) (modified Bellman-Ford using a queue).
//
// Complexity: maybe runtime O(k*e), maybe space O(k+e) [where n = num nodes, e
// = num edges (paths), k = max stops]. Haven't thought about it much.

class Solution {
public:
    int findCheapestPrice(const int n, const vector<vector<int>>& flights,
                          const int src, const int dst, const int k) const
    {
        unordered_multimap<int, pair<int, int>> flightMap;

        for (const auto& path : flights) {
            flightMap.emplace(path[0], pair(path[1], path[2]));
        }

        queue<tuple<int, int, int>> q({{src, 0, 0}});
        int minTotalCost = numeric_limits<int>::max();

        while (!empty(q)) {
            const auto [from, totalCost, steps] = q.front();
            q.pop();

            if (steps > k) {
                continue;
            }

            const auto paths = flightMap.equal_range(from);

            for (auto it = paths.first; it != paths.second; ++it) {
                const auto [to, cost] = it->second;
                const int newTotalCost = totalCost + cost;

                if (newTotalCost < minTotalCost) {
                    // new shorter route found - re-check that city if not dst
                    if (to == dst) {
                        minTotalCost = newTotalCost;
                    } else {
                        q.emplace(to, newTotalCost, steps + 1);
                    }
                }
            }
        }

        return minTotalCost == numeric_limits<int>::max() ? -1 : minTotalCost;
    }
};
