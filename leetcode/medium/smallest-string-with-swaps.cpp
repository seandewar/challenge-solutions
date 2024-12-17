// https://leetcode.com/problems/smallest-string-with-swaps
// Complexity: runtime O(p+slogs), space O(s+p).

class Solution {
    void dfs(const string &s, const vector<vector<int>> &graph,
             vector<int> &visited, const int i, vector<int> &swap_is,
             string &swap_chars) const
    {
        if (exchange(visited[i], true))
            return;

        swap_is.push_back(i);
        swap_chars.push_back(s[i]);
        for (const int j : graph[i])
            dfs(s, graph, visited, j, swap_is, swap_chars);
    }

public:
    string smallestStringWithSwaps(const string &s,
                                   const vector<vector<int>> &pairs) const
    {
        vector<vector<int>> graph(s.size());
        for (const auto &p : pairs) {
            if (p[0] == p[1])
                continue; // pointless self-connection (but harmless)
            graph[p[0]].push_back(p[1]);
            graph[p[1]].push_back(p[0]);
        }

        vector<int> visited(s.size()), swap_is;
        string swap_chars, ret = s;
        for (int i = 0; i < s.size(); ++i) {
            swap_is.clear();
            swap_chars.clear();
            dfs(s, graph, visited, i, swap_is, swap_chars);
            sort(swap_is.begin(), swap_is.end());
            sort(swap_chars.begin(), swap_chars.end());
            for (int j = 0; j < swap_is.size(); ++j)
                ret[swap_is[j]] = swap_chars[j];
        }
        return ret;
    }
};
