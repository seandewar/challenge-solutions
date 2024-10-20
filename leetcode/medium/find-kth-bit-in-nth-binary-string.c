// https://leetcode.com/problems/find-kth-bit-in-nth-binary-string
//
// Unlike the alternative solution that looks at all possible branches, we can
// simply search just the relevant branch if we work down from the requested n,
// adjusting k for each level until we have a known bit value (e.g: middle bit
// is always '1', n == 1 has only one bit, which is always '0'), then apply the
// changes to it as we bubble back up the call stack.
//
// Complexity: runtime O(n), space O(n).

char findKthBit(const int n, const int k)
{
    const int len = (1 << n) - 1, m = (len + 1) / 2;
    if (n == 1)
        return '0';
    if (k == m)
        return '1';
    if (k < m)
        return findKthBit(n - 1, k);

    return findKthBit(n - 1, len - k + 1) == '0' ? '1' : '0'; // k > m
}

// Alternative Solution: Initial solution: depth-first recursive; all branches.
// Complexity: runtime O(2^n), space O(n).

static int dfs(const int d, const int i, const int x, const int n, const int k)
{
    if (d == n)
        return i == k ? x : -1;

    const int len = (1 << d) - 1, l = dfs(d + 1, i, x, n, k),
              m = i == len ? dfs(d + 1, i + 1, 1, n, k) : -1,
              r = dfs(d + 1, 2 * len - i + 2, !x, n, k);
    return l != -1 ? l : (m != -1 ? m : r);
}

char findKthBit(const int n, const int k)
{
    return dfs(1, 1, 0, n, k) + '0';
}
