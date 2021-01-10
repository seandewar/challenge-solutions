// https://leetcode.com/problems/word-ladder/
//
// Depth-first search on graph solution.
//
// This solution constructs a graph, starting at node 'beginWord', containing
// nodes of words from 'wordList'. Nodes for words A and B are connected with a
// directed edge A -> B if there exists a single-character change we can make to
// word A that produces word B; the 'graphAddWord()' function generates these
// edges in the graph after adding a new word node A.
//
// To find the shortest path, a BFS is used on the graph. (There is no need for
// any fancy algorithm such as Dijkstra's, etc. as each edge is only of length 1
// to represent a 1 character change in the word; using such an algorithm will
// just devolve into a BFS logically anyway)
//
// Note it is not actually necessary to explicitly create the graph structure
// to solve this problem, but in my opinion, it's easier to think about this
// question in graph theory terms first. The possible words to search next can
// be generated "on the fly" from within the BFS loop, without the need for
// pre-generating any graph.
//
// Complexity: runtime O(TODO), space O(TODO).

class Solution {
    void graphAddWord(unordered_multimap<string_view, string_view>& graph,
                      const unordered_set<string_view>& wordSet,
                      const string_view fromWord) const
    {
        string w(fromWord);

        for (size_t i = 0; i < size(w); ++i) {
            for (w[i] = 'a'; w[i] <= 'z'; ++w[i]) {
                if (w[i] == fromWord[i])
                    continue;

                if (const auto fIt = wordSet.find(w); fIt != cend(wordSet))
                    graph.emplace(fromWord, *fIt);
            }

            w[i] = fromWord[i];
        }
    }

public:
    int ladderLength(const string& beginWord, const string& endWord,
                     const vector<string>& wordList) const
    {
        const unordered_set<string_view> wordSet(cbegin(wordList),
                                                 cend(wordList));

        unordered_multimap<string_view, string_view> graph;
        graphAddWord(graph, wordSet, beginWord);
        for (const auto& w : wordList)
            graphAddWord(graph, wordSet, w);

        // BFS. kinda same as idea as any shortest-path algo here, but no need
        // to individually track the current min steps to each node; as for each
        // step taken, every path queued will have the same current distance
        // covered in steps (due to the breadth-first nature of the search).
        // also why we can terminate immediately upon first finding a path to
        // the endWord node, as the step count at that point will be the minimum
        unordered_set<string_view> visitedSet;
        visitedSet.reserve(size(wordSet));
        queue<string_view> bfsQueue;
        bfsQueue.push(beginWord);

        for (int stepCount = 1; !empty(bfsQueue); ++stepCount) {
            const auto stepSize = size(bfsQueue);
            for (size_t i = 0; i < stepSize; ++i) {
                const auto word = bfsQueue.front();
                bfsQueue.pop();

                if (word == endWord)
                    return stepCount; // endWord found; return num steps

                if (!visitedSet.insert(word).second)
                    continue; // already visited this node; skip

                const auto range = graph.equal_range(word);
                for (auto it = range.first; it != range.second; ++it)
                    bfsQueue.push(it->second);
            }
        }

        return 0; // all nodes visited; no path to endWord
    }
};
