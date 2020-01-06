// https://leetcode.com/problems/climbing-stairs/
//
// Dynamic Programming Recursive Solution (depth-first search, top-down)
// Complexity: runtime O(n), space O(n).

class Solution {
    int dfs(int n, vector<int>& cache)
    {
        // if dfs(1) we need 1 step.
        // if dfs(2) then dfs(1) + dfs(0) gets called; we need 2 steps
        if (n < 2)
            return 1;

        // already calculated the number of ways to get to the top from n steps
        // away; return this cached result
        if (cache[n - 1] > 0)
            return cache[n - 1];

        cache[n - 1] = dfs(n - 1, cache) + dfs(n - 2, cache);
        return cache[n - 1];
    }

public:
    int climbStairs(int n)
    {
        vector<int> solutionCache(n);
        return dfs(n, solutionCache);
    }
};
