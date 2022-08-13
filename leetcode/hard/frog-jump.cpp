// https://leetcode.com/problems/frog-jump
// Recursion + DP (Memoization)

class Solution {
    bool dfs(vector<unordered_set<int>> &mem, vector<int> &stones, int i,
             int prevJump)
    {
        if (i == size(stones) - 1)
            return true;
        if (!mem[i].insert(prevJump).second)
            return false;

        for (int jump = max(1, prevJump - 1); jump <= prevJump + 1; ++jump) {
            int nextStone = stones[i] + jump;
            auto it =
                lower_bound(begin(stones) + i + 1, end(stones), nextStone);
            if (it != end(stones) && *it == nextStone
                && dfs(mem, stones, it - begin(stones), jump))
                return true;
        }
        return false;
    }

public:
    bool canCross(vector<int> &stones)
    {
        if (stones[0] + 1 != stones[1])
            return false;

        vector<unordered_set<int>> mem(size(stones));
        return dfs(mem, stones, 1, 1);
    }
};
