// https://leetcode.com/problems/cut-off-trees-for-golf-event
//
// BFS (Dijkstra's degenerates into this when the distance between any adjacent
// square is the same everywhere).
//
// Same idea as any other shortest-path problem, but just restart the search,
// looking for the next target instead, once the current target has been
// visited.
//
// Some room for optimization here; for example, we could store already-visited
// squares in an unordered_set or something, rather than negate map squares and
// re-iterate the entire map to un-mark them after every search iteration.
//
// Complexity: runtime O(t*m*n*log(m*n)), space O(m*n). [where m=rows, n=cols,
// t=num trees]

class Solution {
public:
    int cutOffTree(vector<vector<int>>& forest) const
    {
        // we'll be travelling from the shortest tree to the tallest, so
        // enumerate the possible tree heights and sort ascending
        vector<int> treeHeights;
        for (const auto& row : forest) {
            copy_if(cbegin(row), cend(row), back_inserter(treeHeights),
                    [](const int h) { return h > 1; });
        }
        sort(begin(treeHeights), end(treeHeights));

        auto targetHeightIt = cbegin(treeHeights);
        queue<pair<size_t, size_t>> q;
        q.emplace(0, 0);

        for (int dist = 0; !empty(q); ++dist) {
            const auto queueSize = size(q);
            for (size_t i = 0; i < queueSize; ++i) {
                const auto [x, y] = q.front();
                q.pop();

                if (y >= size(forest) || x >= size(forest[0]))
                    continue; // out of bounds; ignore
                if (forest[y][x] <= 0)
                    continue; // wall (0) or already visited (-ve); ignore

                // check if this is our target tree
                if (forest[y][x] == *targetHeightIt) {
                    if (++targetHeightIt == cend(treeHeights))
                        return dist; // all trees were visited; we're finished!

                    // reached our target, so restart our search from this
                    // position, un-marking already visited trees (we may need
                    // to traverse through them again to get to the next target)
                    for (auto& row : forest) {
                        transform(begin(row), end(row),
                                  [](const int h) { return h < 0 ? -h : h; });
                    }

                    // our target was reached, so we don't need to consider
                    // candidate paths from the previous search iteration any
                    // more; clear the queue (queue doesn't have a clear()
                    // method), restart search from this position, and decrement
                    // the distance travelled (as breaking here to restart the
                    // search will increment it, but we haven't actually moved
                    // yet; we're restarting from the same position)
                    for (; !empty(q); q.pop());
                    q.emplace(x, y);
                    --dist;
                    break;
                }

                // mark position as visited and queue the 4 adjacent directions
                // for a visit on the next search iteration
                forest[y][x] *= -1;
                q.emplace(x + 1, y);
                q.emplace(x - 1, y);
                q.emplace(x, y + 1);
                q.emplace(x, y - 1);
            }
        }

        return -1; // some tree is unreachable; fail
    }
};
