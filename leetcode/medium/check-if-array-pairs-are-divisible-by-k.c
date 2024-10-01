// https://leetcode.com/problems/check-if-array-pairs-are-divisible-by-k
//
// Faster than 100% of solutions. We abuse mod here when constructing the lookup
// table, as we only care about whether the numbers divide into k, not the
// result of the divisions (this also trivializes the search space).
//
// Mod of 0 (numbers that divide exactly) are treated special, as they can only
// be paired with other such numbers that occupy the same mod 0 slot; to make
// pairs, the count of such numbers must be even.
//
// Complexity: runtime O(n), space O(1). [technically O(n+k), O(k), but we're
//                                        bounded by the constraints]

bool canArrange(int *const arr, const int n, const int k)
{
    int lookup[100000] = {0};
    for (int i = 0; i < n; ++i)
        ++lookup[(arr[i] % k) + (arr[i] % k < 0) * k];

    for (int x = 1; x < k; ++x) {
        if (lookup[x] != lookup[k - x])
            return false;
    }

    return lookup[0] % 2 == 0;
}
