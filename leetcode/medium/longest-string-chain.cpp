// https://leetcode.com/problems/longest-string-chain
//
// DFS + DP. This has room for optimization, but I don't have much time. :(
// Complexity: too busy. :(

class Solution {
    int dfs(const unordered_multimap<string_view, string_view>& graph,
            const unordered_map<string_view, int>& mem, string_view w) const
    {
        if (mem[w] != 0)
            return mem[w];

        auto [it, end] = graph.equal_range(w);
        if (it == end)
            return mem[w] = 1;

        int maxChain = 0;
        for (; it != end; ++it)
            maxChain = max(maxChain, 1 + dfs(graph, mem, it->second));

        return mem[w] = maxChain;
    }

public:
    int longestStrChain(const vector<string>& words) const
    {
        unordered_multimap<string_view, string_view> graph;
        for (auto& w : words) {
            for (auto& w2 : words) {
                if (size(w) <= size(w2))
                    continue;

                auto [it, it2] = mismatch(begin(w), end(w), begin(w2), end(w2));
                auto [fit, fit2] = mismatch(it + 1, end(w), it2, end(w2));
                if (fit == end(w) && fit2 == end(w2))
                    graph.emplace(w2, w);
            }
        }

        unordered_map<string_view, int> mem;
        mem.reserve(size(words));
        int maxChain = 0;
        for (auto& w : words)
            maxChain = max(maxChain, dfs(graph, mem, w));

        return maxChain;
    }
};
