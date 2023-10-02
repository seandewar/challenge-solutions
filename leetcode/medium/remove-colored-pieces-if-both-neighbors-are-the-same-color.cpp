// https://leetcode.com/problems/remove-colored-pieces-if-both-neighbors-are-the-same-color
// Complexity: runtime O(n), space O(1).

class Solution {
public:
    bool winnerOfGame(string colors)
    {
        array<int, 2> moves{};
        for (auto it = colors.begin(); it != colors.end();) {
            auto it2 = find(it, colors.end(), *it == 'A' ? 'B' : 'A');
            moves[*it - 'A'] += max<int>(0, it2 - it - 2);
            it = it2;
        }
        return moves[0] > moves[1];
    }
};
