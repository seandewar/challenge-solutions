// https://leetcode.com/problems/minimum-number-of-vertices-to-reach-all-nodes
// Complexity: runtime O(n), space O(n) counting output, O(1) otherwise.

class Solution {
public:
    vector<int>
    findSmallestSetOfVertices(const int n,
                              const vector<vector<int>> &edges) const
    {
        vector<char> unreachable(n, true);
        for (const auto &e : edges)
            unreachable[e[1]] = false;

        vector<int> res;
        for (int i = 0; i < n; ++i) {
            if (unreachable[i])
                res.push_back(i);
        }
        return res;
    }
};
