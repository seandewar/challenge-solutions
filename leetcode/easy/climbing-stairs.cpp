// https://leetcode.com/problems/climbing-stairs/
//
// Iterative solution using 2 variables. Same mechanics may also be applied in
// generating the fibonnaci sequence, and the numbers generated are the same
// starting from the 2nd fibonnaci number (1, 2, 3, 5, 8...).
//
// The tie(curr, prev) line may be replaced with:
//
// prev = curr + prev;
// swap(prev, curr);
//
// Good compilers will presumably generate the same code with optimizations on
// as above, creating only 1 temporary at most.
//
// Complexity: runtime O(n), space O(1).

class Solution {
public:
    int climbStairs(int n)
    {
        if (n == 1)
            return 1;

        int curr = 2;
        int prev = 1;

        for (int i = 2; i < n; ++i)
            tie(curr, prev) = make_tuple(curr + prev, curr);

        return curr;
    }
};

// Dynamic Programming Iterative Solution (bottom-up)
// Complexity: runtime O(n), space O(n).
/*
class Solution {
public:
    int climbStairs(int n)
    {
        // max() used as a quick-fix for n==0 or 1 so that the assignments
        // aren't OOB
        vector<int> solutionCache(max(n, 2));
        solutionCache[0] = 1;
        solutionCache[1] = 2;

        for (int i = 2; i < n; ++i)
            solutionCache[i] = solutionCache[i - 1] + solutionCache[i - 2];

        // same as back() for n>=2, but because of the quick-fix above we
        // cannot use back() here for n==0 or 1
        return solutionCache[n - 1];
    }
};
*/

// Dynamic Programming Recursive Solution (depth-first search, top-down)
// Complexity: runtime O(n), space O(n).
/*
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
*/
