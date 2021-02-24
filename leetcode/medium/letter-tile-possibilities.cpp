// https://leetcode.com/problems/letter-tile-possibilities
//
// Complexity: runtime O(26^n), space O(26) == O(1).

class Solution {
    int dfs(array<size_t, 26>& charCounts, const int combSize) const noexcept
    {
        if (combSize == 0)
            return 0;

        int result = 0;
        for (auto& count : charCounts) {
            if (count == 0)
                continue;

            --count;
            result += 1 + dfs(charCounts, combSize - 1);
            ++count;
        }

        return result;
    }

public:
    int numTilePossibilities(const string& tiles) const noexcept
    {
        array<size_t, 26> charCounts{};
        for (const char c : tiles)
            ++charCounts[c - 'A'];

        return dfs(charCounts, size(tiles));
    }
};
