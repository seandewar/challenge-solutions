// https://leetcode.com/problems/two-sum/
//
// Complexity: runtime O(n), space O(n).

// misses > hits, so spare 128KB for l33t bloom filter h4x
#define HASH_BLOOM 20

struct hash {
    int key;
    int lasti;
    UT_hash_handle hh;
};

// leetcode wants returned array to be allocated (yikes)
int *twoSum(const int *restrict nums, const int n, const int target,
            int *restrict retn)
{
    struct hash items[10000], *ht = NULL;  // overflow stack for great benefit
    for (int i = 0; i < n; ++i) {
        struct hash *h;
        HASH_FIND_INT(ht, &nums[i], h);
        if (h) {
            h->lasti = i;
        } else {
            items[i] = (struct hash) {.key = nums[i], .lasti = i};
            HASH_ADD_INT(ht, key, &items[i]);
        }
    }

    for (int i = 0; i < n; ++i) {
        const int need = target - nums[i];
        const struct hash *h;
        HASH_FIND_INT(ht, &need, h);
        if (h && (nums[i] != need || h->lasti != i)) {
            *retn = 2;  // always 2 elements but leetcode dumb xd
            int *const ret = malloc(2 * sizeof(int));
            ret[0] = i;
            ret[1] = h->lasti;
            return ret;
        }
    }

    abort();  // impossible if inputs aren't messed up
}
