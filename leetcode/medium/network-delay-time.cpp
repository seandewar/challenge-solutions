// https://leetcode.com/problems/network-delay-time/
//
// This solution uses a depth-first search.
//
// Complexity: runtime O(n^n + m*logm), space O(n+m) [where n==number of nodes,
// m==number of edges (size(times))].

class Solution {
    void dfs(const unordered_map<int, vector<pair<int, int>>>& times,
             unordered_map<int, int>& minTimes,
             const int root, const int elapsedTime = 0) const
    {
        // if we found a path through this node earlier with a lower or equal
        // total weight (elapsed time), then this path is suboptimal, and then
        // there is no need to continue considering it
        if (const auto fIt = minTimes.find(root);
            fIt != cend(minTimes) && fIt->second <= elapsedTime)
            return;

        minTimes[root] = elapsedTime;

        // if this node has any outgoing edges, consider including them as part
        // of this path
        if (const auto fIt = times.find(root); fIt != cend(times)) {
            for (const auto x : fIt->second)
                dfs(times, minTimes, x.first, elapsedTime + x.second);
        }
    }

public:
    int networkDelayTime(const vector<vector<int>>& times, const int N,
                         const int K) const
    {
        // convert the input into a map so we can do a constant time (average
        // case) lookup of the outgoing edges for any nodes that have them
        unordered_map<int, vector<pair<int, int>>> timesMap;

        for (const auto& x : times)
            timesMap[x[0]].emplace_back(x[1], x[2]);

        // sort edges in ascending order of weight (time needed). this
        // optimizes runtime enough to pass the test cases, as the overall
        // shortest path to all nodes will likely include most of the shortest
        // edges between nodes
        for (auto& x : timesMap) {
            sort(begin(x.second), end(x.second),
                 [](const auto& a, const auto& b) {
                     return a.second < b.second;
                 });
        }

        unordered_map<int, int> minTimesMap;
        dfs(timesMap, minTimesMap, K);

        if (size(minTimesMap) != N)
            return -1; // failed to find a path through all nodes

        // the path of largest total weight (elapsed time) will be the shortest
        // path that visits every node
        return max_element(cbegin(minTimesMap), cend(minTimesMap),
                           [](const auto& a, const auto& b) {
                               return a.second < b.second;
                           })->second;
    }
};
