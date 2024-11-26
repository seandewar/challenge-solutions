// https://leetcode.com/problems/find-champion-ii
// Faster than 100%. Pretty easy problem for a medium.
// Complexity: runtime O(n+m), space O(n) [where n == max n, m == edges.size()].

class Solution {
public:
    int findChampion(const int n, const vector<vector<int>> &edges) const
    {
        array<bool, 100> in_edges{};
        for (const auto &v : edges)
            in_edges[v[1]] = true;

        const auto end_it = in_edges.begin() + n,
                   it = find(in_edges.begin(), end_it, false);
        return find(it + 1, end_it, false) != end_it ? -1
                                                     : it - in_edges.begin();
    }
};
