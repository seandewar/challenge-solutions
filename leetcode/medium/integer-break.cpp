// https://leetcode.com/problems/integer-break
// Reject maffs; return to memoization!
// Complexity: runtime O(n^2), space O(n).

class Solution {
  int dfs(auto &memo, int n, int x) {
    if (memo[n] != 0)
      return x * memo[n];
    int best = n;
    for (int next = 1; next < n; ++next)
      best = max(best, dfs(memo, n - next, next));
    return x * (memo[n] = best);
  }

public:
  int integerBreak(int n) {
    if (n < 4)
      return n - 1;
    array<int, 59> memo{};
    return dfs(memo, n, 1);
  }
};
