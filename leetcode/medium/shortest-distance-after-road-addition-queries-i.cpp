// https://leetcode.com/problems/shortest-distance-after-road-addition-queries-i
// Complexity: runtime O(n+q*(q+n)), space O(N+q) [where N == max n].

class Solution {
public:
  vector<int>
  shortestDistanceAfterQueries(const int n,
                               const vector<vector<int>> &queries) const {
    vector<int> ret;
    ret.reserve(queries.size());

    unordered_multimap<int, int> graph;
    array<int, 501> memo; // +1 removes need for bounds checking memo[i + 1]
    iota(memo.begin(), memo.begin() + n + 1, 0);
    for (const auto &v : queries) {
      graph.emplace(v[0], v[1]);

      if (memo[v[0]] + 1 < memo[v[1]]) {
        memo[v[1]] = memo[v[0]] + 1;
        // j the upper-bound for distances that need relaxing; a queue would
        // probably be faster instead, but this gives us better than 91% runtime
        // anyway, while being simple.
        for (int i = v[1], j = i; i <= j && i < n; ++i) {
          const auto [it0, it1] = graph.equal_range(i);
          for (auto it = it0; it != it1; ++it) {
            if (memo[i] + 1 < memo[it->second]) {
              memo[it->second] = memo[i] + 1;
              j = max(j, it->second);
            }
          }
          if (memo[i] + 1 < memo[i + 1]) {
            memo[i + 1] = memo[i] + 1;
            j = max(j, i + 1);
          }
        }
      }

      ret.push_back(memo[n - 1]);
    }

    return ret;
  }
};
