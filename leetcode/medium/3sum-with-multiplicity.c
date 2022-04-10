/* https://leetcode.com/problems/3sum-with-multiplicity */
/*
 * We can count the occurrences of each number, and use an approach similar to 2
 * sum + some combinatorics.
 *
 * If a tuple (x, y, z) has 3 different numbers, the number of ways to make this
 * tuple is counts[x] * counts[y] * counts[z].
 *
 * However, if there are repeating numbers, the above formula breaks. For
 * example, if x == z, the tuple (x, y, z) and (z, y, x) are the same, so we
 * don't want to count this combination multiple times, as the above formula
 * would do. Instead, we can use the nCr function to choose 2 values of x (or z)
 * from a total of counts[x] (or counts[z]) where order doesn't matter, and
 * multiply that with counts[y].
 *
 * Following from this, if x == y == z, the answer is just nC3 where
 * n = counts[x] (or counts[y] or counts[z]).
 *
 * Note that as an optimization, we can choose to skip calculating "ret" in some
 * cases when z == y or z == x, e.g:
 *
 *  - for all x, y, z, skip if counts[z] == 0,
 *  - for x == y == z, skip if counts[z] < 3,
 *  - for x != y == z, skip if counts[z] < 2.
 *
 *  Note that x == z isn't possible, as y always begins counting at x and we
 *  skip if z < y. If we didn't do this, we'd count tuples like (1, 2, 1)
 *  multiple times; instead, this tuple would be counted as (1, 1, 2).
 */
/* Complexity: runtime O(n+101^2) == O(n), space O(101) == O(1). */

#define MOD 1000000007

static inline uint64_t fact(int n, const int after)
{
    uint64_t ret = 1;
    for (; n > after; --n)
        ret *= (uint64_t)n;
    return ret;
}

static inline int ncr(const int n, const int r)
{
    return (int)((fact(n, n - r) / fact(r, 1)) % MOD);
}

int threeSumMulti(const int *const arr, const int n, const int target)
{
    int counts[101] = {0};
    for (size_t i = 0; i < n; ++i)
        ++counts[arr[i]];

    int ret = 0;
    for (int x = 0; x < 101; ++x) {
        for (int y = x; y < 101; ++y) {
            const int z = target - x - y;
            if (z < y || z > 100)
                continue;

            if (x == y && y == z) // x == y == z
                ret += ncr(counts[z], 3);
            else if (x == y) // x == y != z
                ret += counts[z] * ncr(counts[y], 2);
            else if (y == z) // x != y == z
                ret += counts[x] * ncr(counts[z], 2);
            else // x != y != z
                ret += counts[x] * counts[y] * counts[z];
            ret %= MOD;
        }
    }

    return ret;
}
