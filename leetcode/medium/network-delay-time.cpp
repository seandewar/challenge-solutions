// https://leetcode.com/problems/network-delay-time/
//
// The solutions to this problem rely on using a shortest-path algorithm to
// find the shortest path connecting all nodes in the graph from node K.
//
// Solutions to this problem can almost be thought of as a simulation of the
// "signal"'s propagation between nodes in the graph starting from some node K.
// When the signal reaches a node, we only record the minimum time it took to
// do so, as the same signal can reach the same node multiple times at
// different moments via another path; we model this using edge relaxations,
// whereby we overwrite the recorded total time needed to reach a node with
// that of the shorter candidate path we found to it, searching again from
// this node while accounting for our new candidate path's reduced total time.
//
// This solution uses a variation of depth-first search, modified to use edge
// relaxations.
//
// The edges are sorted in ascending weight order, as due to the nature of DFS,
// which commits its search to an entire chain/branch of edges before
// backtracking, we will be more likely to find the solution quicker if we
// commit to a candidate path of a shorter total length (otherwise, we can
// waste a lot of time committing to longer candidate paths first, only to find
// shorter ones later via edge relaxations).
//
// Complexity: runtime O(m^n + m*logm), space O(n+m) [where n==number of nodes,
// m==number of edges (size(times))].

class Solution {
    void dfs(const unordered_map<int, vector<pair<int, int>>>& times,
             unordered_map<int, int>& minTimes,
             const int root, const int elapsedTime = 0) const
    {
        // if we found a path through this node earlier with a lower or equal
        // total weight (elapsed time), then this path is suboptimal, and then
        // there is no need to continue considering it (edge relaxation)
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
        // edges in the graph
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
        // path that visits every node. UB if empty(times) and N==0, but we
        // never check for this as the test inputs guarantee at least 1 node
        return max_element(cbegin(minTimesMap), cend(minTimesMap),
                           [](const auto& a, const auto& b) {
                               return a.second < b.second;
                           })->second;
    }
};

// Alternative Solution: Shortest Path Faster Algorithm (SPFA) (modified
// Bellman-Ford with a queue AKA modified breadth-first search with edge
// relaxations).
//
// Unlike DFS, which continuously visits a chain/branch of edges as far as
// possible before backtracking, BFS uses a queue to visit all the outgoing
// edges of a particular node first before moving on to all of the outgoing
// edges of another node (hence the name: breadth vs depth). This has the
// effect of expanding the search area somewhat evenly in all directions (in
// depth order) rather than committing to an entire path of nodes before
// backtracking.
//
// Because of this, the sorting of edges beforehand isn't as necessary, as
// even if we visit an outgoing edge that isn't of minimum weight first, we
// will take turns visiting the other edges of the same node immediately
// afterwards (instead of committing to all the outgoing paths of the initial
// choice first before backtracking), which includes the minimum weight edge.
//
// Complexity: runtime O(m*n), space O(n+m) [minTimesMap/queue/qset + timesMap].
// [where n==number of nodes, m==number of edges (size(times))]
/*
class Solution {
public:
    int networkDelayTime(const vector<vector<int>>& times, const int N,
                         const int K) const
    {
        unordered_map<int, vector<pair<int, int>>> timesMap;

        for (const auto& x : times)
            timesMap[x[0]].emplace_back(x[1], x[2]);

        unordered_map<int, int> minTimesMap = {{K, 0}};

        queue<int> q({K});
        unordered_set qset = {K}; // O(1) lookup for what nodes are in the queue

        while (!empty(q)) {
            const int root = q.front();
            qset.erase(root);
            q.pop();

            if (const auto fIt = timesMap.find(root);
                fIt != cend(timesMap)) {
                const int minTime = minTimesMap[root];

                for (const auto& x : fIt->second) {
                    const auto [next, time] = x;

                    // check if we just found a shorter path to this node. if
                    // so, perform edge relaxation
                    if (const auto fIt = minTimesMap.find(next);
                        fIt == cend(minTimesMap)
                        || minTime + time < fIt->second) {
                        minTimesMap[next] = minTime + time;

                        // don't queue if this node is already in the queue
                        if (qset.count(next) > 0)
                            continue;

                        q.push(next);
                        qset.insert(next);
                    }
                }
            }
        }

        if (size(minTimesMap) != N)
            return -1;

        return max_element(cbegin(minTimesMap), cend(minTimesMap),
                           [](const auto& a, const auto& b) {
                               return a.second < b.second;
                           })->second;
    }
};
*/

// Alternative Solution: Dijkstra's algorithm (similar to Bellman-Ford + a
// priority queue).
//
// Somewhat similar to the above SPF algorithm, except we use a min priority
// queue (min heap) instead of a regular FIFO queue. This heap is used to
// greedily continue our search via the shortest total weight candidate path
// that we have found so far, meaning that if we have already visited a node,
// we did so via the shortest possible path from K.
//
// NOTE: As std::priority_queue does not support decrease-key operations (we
// can't edit any key in the heap to be a lesser value in at most logarithmic
// time), and because we can't see if a key already exists in the heap without
// implementing some lookup structure, multiple candidate path lengths to the
// same node may be stored within the heap instead of only the shortest
// available. If decrease-key is supported, the worst-case runtime can be
// reduced to O(nlogn + m) with some modifications, as each node will have only
// its shortest candidate path to it within the heap, instead of a candidate
// path for each ingoing edge to that node (O(n) vs O(m) entries).
//
// Complexity: runtime O(mlogm), space O(n+m) [minTimesMap + timesMap/heap].
// [where n==number of nodes, m==number of edges (size(times))]
/*
class Solution {
public:
    int networkDelayTime(const vector<vector<int>>& times, const int N,
                         const int K) const
    {
        unordered_map<int, vector<pair<int, int>>> timesMap;

        for (const auto& x : times)
            timesMap[x[0]].emplace_back(x[1], x[2]);

        unordered_map<int, int> minTimesMap;

        // NOTE: GCC fails at deducing q's type from "priority_queue
        // q(<lambda>, vector{pair(K, 0)})" clang deduces this fine, though.
        // unforunately, leetcode uses GCC :(
        const auto comp = [](const auto& a, const auto& b) {
                              return a.second > b.second;
                          };

        priority_queue<pair<int, int>, vector<pair<int, int>>, decltype(comp)>
            q(comp, {{K, 0}});

        while (!empty(q)) {
            const auto [root, elapsedTime] = q.top();
            q.pop();

            // pre-condition: if we already visited this node, we would have
            // already done via the shortest path from K
            if (minTimesMap.count(root) > 0)
                continue;

            minTimesMap[root] = elapsedTime;

            if (const auto fIt = timesMap.find(root); fIt != cend(timesMap)) {
                for (const auto x : fIt->second) {
                    // avoid unnecessary O(logn) insert by also checking the
                    // pre-condition here
                    if (minTimesMap.count(x.first) == 0)
                        q.emplace(x.first, elapsedTime + x.second);
                }
            }
        }

        if (size(minTimesMap) != N)
            return -1;

        return max_element(cbegin(minTimesMap), cend(minTimesMap),
                           [](const auto& a, const auto& b) {
                               return a.second < b.second;
                           })->second;
    }
};
*/
