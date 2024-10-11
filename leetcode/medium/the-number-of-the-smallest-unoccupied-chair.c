// https://leetcode.com/problems/the-number-of-the-smallest-unoccupied-chair
// Complexity: runtime O(nlogn), space O(n).

static const int *const *cmp_times;
static int cmp(const void *const a, const void *const b)
{
    const int *const atimes = cmp_times[*(const int *)a],
                     *const btimes = cmp_times[*(const int *)b];
    return atimes[0] - btimes[0];
}

struct Node {
    int k, v;
};

struct Heap {
    struct Node nodes[10000];
    int len;
};

static void heap_push(struct Heap *const h, const struct Node *const node)
{
    int i = h->len++;
    for (; i > 0;) {
        const int parenti = (i - 1) / 2;
        if (h->nodes[parenti].k < node->k)
            break;
        h->nodes[i] = h->nodes[parenti];
        i = parenti;
    }
    h->nodes[i] = *node;
}

static void heap_pop(struct Heap *const h)
{
    --h->len;
    int i = 0;
    while (true) {
        const int li = 2 * i + 1, ri = li + 1,
                  lend = li < h->len ? h->nodes[li].k : INT_MAX,
                  rend = ri < h->len ? h->nodes[ri].k : INT_MAX;
        if (lend > h->nodes[h->len].k && rend > h->nodes[h->len].k)
            break;

        const int nexti = lend < rend ? li : ri;
        h->nodes[i] = h->nodes[nexti];
        i = nexti;
    }
    h->nodes[i] = h->nodes[h->len];
}

int smallestChair(const int *const *const times, const int n,
                  const int *const _ /* always 2 */, const int targeti)
{
    int is[10000];
    for (int i = 0; i < n; ++i)
        is[i] = i;
    cmp_times = times;
    qsort(is, n, sizeof is[0], cmp); // sort friend indices by arrival times

    // Min heap of leave times to used chair indices (time_heap), and free chair
    // indices (chairi_heap; "v" field unused) respectively.
    struct Heap time_heap, chairi_heap;
    time_heap.len = chairi_heap.len = 0;

    for (int i = 0; i < n; ++i) {
        const int friendi = is[i], *const friend_times = times[friendi];
        while (time_heap.len > 0 && time_heap.nodes[0].k <= friend_times[0]) {
            heap_push(&chairi_heap, &(const struct Node){time_heap.nodes[0].v});
            heap_pop(&time_heap);
        }

        int chairi = time_heap.len;
        if (chairi_heap.len > 0) {
            chairi = chairi_heap.nodes[0].k;
            heap_pop(&chairi_heap);
        }
        if (friendi == targeti)
            return chairi;

        heap_push(&time_heap, &(const struct Node){friend_times[1], chairi});
    }
    return -1; // impossible; inputs guarantee a solution
}
