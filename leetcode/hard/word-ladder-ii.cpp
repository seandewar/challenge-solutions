// https://leetcode.com/problems/word-ladder-ii
//
// Generate a graph from the wordList and transformation rules, BFS through the
// graph from the beginWord until we get to the endWord (make sure to consider
// all of the shortest paths!), reconstruct the paths from the data in "prevs"
// (which stores the previous word visited in the shortest paths to a word; this
// avoids needing to keep track of the full path while traversing the graph,
// which avoids TLE) via a DFS with backtracking.
//
// So, to recap: transform wordList into graph via transformation rules, BFS the
// graph, reconstruct the shortest paths via DFS + backtracking -- that's fun!
//
// Previously I had a Rust solution that passed without the path reconstruction
// stuff (while BFSing, the full path was remembered for each word being
// visited), but more test cases were since added that makes that approach TLE.

class Solution {
public:
    vector<vector<string>> findLadders(string beginWord, string endWord,
                                       vector<string> &wordList)
    {
        auto endWordIt = find(begin(wordList), end(wordList), endWord);
        if (endWordIt == end(wordList)) {
            return {};
        }
        int endWordIdx = endWordIt - begin(wordList);
        int beginWordIdx = size(wordList);
        wordList.push_back(beginWord);
        array<vector<int>, 500> graph;
        for (int i = 0; i < size(wordList); ++i) {
            for (int j = i + 1; j < size(wordList); ++j) {
                int diffCount = 0;
                for (int k = 0; k < size(wordList[i]); ++k) {
                    if (wordList[i][k] != wordList[j][k] && ++diffCount > 1) {
                        break;
                    }
                }
                if (diffCount == 1) {
                    graph[i].push_back(j);
                    graph[j].push_back(i);
                }
            }
        }

        array<vector<int>, 501> prevs;
        array<int, 501> dists;
        dists.fill(numeric_limits<int>::max());
        queue<int> q;
        q.push(beginWordIdx);
        for (int dist = 0; !empty(q) && empty(prevs[endWordIdx]); ++dist) {
            int sz = size(q);
            for (int i = 0; i < sz; ++i) {
                auto wordIdx = q.front();
                q.pop();
                for (int j : graph[wordIdx]) {
                    if (dist > dists[j]) {
                        continue;
                    }
                    if (dist < dists[j]) {
                        dists[j] = dist;
                        prevs[j].clear();
                        q.push(j);
                    }
                    prevs[j].push_back(wordIdx);
                }
            }
        }

        vector<vector<string>> ret;
        vector<int> revPath{endWordIdx};
        buildResult(ret, wordList, prevs, beginWordIdx, revPath);
        return ret;
    }

    void buildResult(vector<vector<string>> &ret, vector<string> &wordList,
                     array<vector<int>, 501> &prevs, int beginWordIdx,
                     vector<int> &revPath)
    {
        int wordIdx = revPath.back();
        if (wordIdx == beginWordIdx) {
            vector<string> path;
            path.reserve(size(revPath));
            transform(begin(revPath), end(revPath), back_inserter(path),
                      [&](int i) { return wordList[i]; });
            reverse(begin(path), end(path));
            ret.push_back(move(path));
            return;
        }
        for (int i : prevs[wordIdx]) {
            revPath.push_back(i);
            buildResult(ret, wordList, prevs, beginWordIdx, revPath);
            revPath.pop_back();
        }
    }
};
