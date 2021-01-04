// https://leetcode.com/problems/check-array-formation-through-concatenation/
//
// Hash map solution.
//
// Alternatively, an array of size 101 can be used over the unordered_map here,
// as per the question restraints: "1 <= arr[i], pieces[i][j] <= 100".
//
// Complexity: runtime O(n), space O(n).

class Solution {
public:
    bool canFormArray(const vector<int>& arr,
                      const vector<vector<int>>& pieces) const
    {
        unordered_map<int, reference_wrapper<const vector<int>>> pieceMap;
        pieceMap.reserve(size(pieces));

        for (const auto& p : pieces)
            pieceMap.emplace(p.front(), p);

        for (auto it = cbegin(arr); it != cend(arr);) {
            const auto fIt = pieceMap.find(*it);
            if (fIt == pieceMap.cend())
                return false;

            const auto& p = fIt->second.get();
            if (!equal(p.cbegin(), p.cend(), it))
                return false;

            it += size(p);
        }

        return true;
    }
};

// Alternative Solution: Using a trie.
//
// This solution is overkill, but more extensible than the last (again, an array
// of size 101 can be used over the unordered_map, if wanted).
//
// If the question didn't guarantee that the input arrays must contain distinct
// integers, this solution could be extended to handle that case properly
// (back-tracking & adding some sort of "remaining count" member to TrieNode).
//
// Complexity: runtime O(n), space O(n).

class Solution {
    struct TrieNode {
        unordered_map<int, unique_ptr<TrieNode>> children;
        bool isPieceEnd = false;
    };

public:
    bool canFormArray(const vector<int>& arr,
                      const vector<vector<int>>& pieces) const
    {
        TrieNode root;

        // populate the trie with pieces
        for (const auto& p : pieces) {
            auto node = &root;
            for (const int n : p) {
                auto& child = node->children[n];
                if (!child)
                    child = make_unique<TrieNode>();

                node = child.get();
            }
            node->isPieceEnd = true;
        }

        // iterate through arr while traversing the pieces trie to ensure the
        // parts in arr are made up from these pieces
        auto node = &root;
        for (const int n : arr) {
            const auto fIt = node->children.find(n);
            if (fIt == node->children.cend())
                return false;

            node = fIt->second.get();
            if (node->isPieceEnd)
                node = &root;
        }

        return node == &root;
    }
};
