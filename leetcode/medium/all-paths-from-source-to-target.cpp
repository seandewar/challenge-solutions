// https://leetcode.com/problems/all-paths-from-source-to-target/
//
// Complexity: runtime O(2^n), space O(n) [recursion overhead; output not
// considered].

class Solution {
    void dfsBuildPath(const vector<vector<int>>& graph,
                      vector<vector<int>>& out, const int node = 0,
                      vector<int> currPath = {}) const
    {
        currPath.push_back(node);

        if (node == size(graph) - 1) {
            out.push_back(move(currPath));
            return;
        }

        for (const int n : graph[node]) {
            dfsBuildPath(graph, out, n, currPath);
        }
    }

public:
    vector<vector<int>>
    allPathsSourceTarget(const vector<vector<int>>& graph) const
    {
        vector<vector<int>> result;
        dfsBuildPath(graph, result);
        return result;
    }
};
