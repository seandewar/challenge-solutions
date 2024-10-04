// https://leetcode.com/problems/make-sum-divisible-by-p
// See code comments for details.
// Complexity: runtime O(n), space O(n+p).

struct PrefixEntry {
    int mod, i;
    UT_hash_handle hh;
};

int minSubarray(const int *const nums, const int n, const int p)
{
    int rem = 0;
    for (int i = 0; i < n; ++i)
        rem = (rem + nums[i]) % p; // modulo while summing avoids overflows
    if (rem == 0)
        return 0;

    struct PrefixEntry *ht = NULL, *e = malloc(sizeof *e);
    e->mod = 0;
    e->i = -1; // valid sub-arrays can start from nums[0]
    HASH_ADD_INT(ht, mod, e);

    int mod = 0, best = n;
    for (int i = 0; i < n; ++i) {
        mod = (mod + nums[i]) % p;
        // A valid sub-array ending at i must start from an index one position
        // past the end of the prefix sum (modulo p) with this value. We want
        // the modulo to be from floored division (C uses truncated division);
        // as rem is always less than p, the dividend is offset by p to do that.
        const int need = (mod - rem + p) % p;
        HASH_FIND_INT(ht, &need, e);
        if (e && best > i - e->i)
            best = i - e->i;

        HASH_FIND_INT(ht, &mod, e);
        if (!e) {
            e = malloc(sizeof *e);
            e->mod = mod;
            HASH_ADD_INT(ht, mod, e);
        }
        e->i = i;
    }
    return best == n ? -1 : best;
}
