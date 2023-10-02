// https://leetcode.com/problems/longest-string-chain
//
// DFS + DP. Faster than 99.23%.
// Can maybe reach 100% by inlining some of the recursion logic.
//
// Complexity: time: O(n*m), space: O(n*m).

class Solution {
  int dfs(vector<string> &words, vector<vector<size_t>> &graph,
          vector<int> &mem, size_t i) {
    if (mem[i] != 0)
      return mem[i];

    int maxChain = 1;
    for (auto j : graph[i])
      maxChain = max(maxChain, 1 + dfs(words, graph, mem, j));

    return mem[i] = maxChain;
  }

public:
  int longestStrChain(vector<string> &words) {
    array<vector<size_t>, 17> byLen;
    for (size_t i = 0; i < size(words); ++i)
      byLen[size(words[i]) - 1].push_back(i);

    vector<vector<size_t>> graph(size(words));
    vector<int> mem(size(words));

    for (size_t i = 0; i < size(words); ++i) {
      auto &w = words[i];

      for (auto j : byLen[size(w)]) {
        auto &w2 = words[j];

        auto [it, it2] = mismatch(begin(w), end(w), begin(w2), end(w2));
        auto [fit, fit2] = mismatch(it, end(w), it2 + 1, end(w2));
        if (fit == end(w) && fit2 == end(w2))
          graph[i].push_back(j);
      }
    }

    int maxChain = 0;
    for (size_t i = 0; i < size(words); ++i)
      maxChain = max(maxChain, dfs(words, graph, mem, i));

    return maxChain;
  }
};
