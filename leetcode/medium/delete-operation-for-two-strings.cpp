// https://leetcode.com/problems/delete-operation-for-two-strings
//
// Complexity: runtime O(w1*w2) I guess, but I didn't think about it,
//             space O(w1*w2).

class Solution {
    int dfs(string_view w1, string_view w2, vector<vector<int>>& mem) const
    {
        auto& m = mem[size(w1)][size(w2)];
        if (m != -1)
            return m;

        if (empty(w1) || empty(w2))
            return m = max(size(w1), size(w2));

        if (w1[0] == w2[0])
            return m = dfs(w1.substr(1), w2.substr(1), mem);

        return m = 1
                   + min(dfs(w1.substr(1), w2, mem),
                         dfs(w1, w2.substr(1), mem));
    }

public:
    int minDistance(string w1, string w2) const
    {
        vector<vector<int>> mem(size(w1) + 1, vector(size(w2) + 1, -1));
        return dfs(w1, w2, mem);
    }
};
