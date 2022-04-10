/* https://leetcode.com/problems/number-of-provinces */
/*  */
/* Complexity: runtime O(n^2), space O(1). */

static int dfs(int *const restrict *const map, const int n, const int i)
{
    if (map[i][i] == -1)
        return 0;

    map[i][i] = -1;
    for (int j = 0; j < n; ++j) {
        if (map[i][j] == 1)
            dfs(map, n, j);
    }
    return 1;
}

int findCircleNum(int *const restrict *const map, const int m,
                  int *const restrict ns)
{
    int ret = 0;
    for (int i = 0; i < m; ++i)
        ret += dfs(map, m, i);

    return ret;
}
