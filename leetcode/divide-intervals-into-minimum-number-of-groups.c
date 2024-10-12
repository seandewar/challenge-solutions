// https://leetcode.com/problems/divide-intervals-into-minimum-number-of-groups
// Event-based.
// Complexity: runtime O(nlogn), space O(n).

struct Event {
    int t, d;
};

static int cmp(const void *const a, const void *const b)
{
    const struct Event *const ea = a, *const eb = b;
    if (ea->t != eb->t)
        return ea->t - eb->t;
    return eb->d - ea->d;
}

int minGroups(const int *const *const ivs, const int n, const int *const _)
{
    struct Event events[200000] = {0};
    for (int i = 0; i < n; ++i) {
        const int j = 2 * i;
        events[j] = (const struct Event){ivs[i][0], 1};
        events[j + 1] = (const struct Event){ivs[i][1], -1};
    }
    qsort(events, 2 * n, sizeof events[0], cmp);
    int overlap = 0, ret = 0;
    for (int i = 0; i < 2 * n; ++i) {
        overlap += events[i].d;
        if (overlap > ret)
            ret = overlap;
    }
    return ret;
}

// Alternative Solution: simple line sweep-based solution (less space efficient
// than a standard line sweep for this question, and can be improved by culling
// the range of numbers outside the min and max interval values, but better if
// the range of numbers > number of intervals).
// Complexity: runtime O(n+k), space O(k).

int minGroups(const int *const *const ivs, const int n, const int *const _)
{
    int events[1000002] = {0};
    for (int i = 0; i < n; ++i) {
        ++events[ivs[i][0]];
        --events[ivs[i][1] + 1];
    }
    int overlap = 0, ret = 0;
    for (int i = 1; i < 1000002; ++i) {
        overlap += events[i];
        if (overlap > ret)
            ret = overlap;
    }
    return ret;
}

// Alternative Solution: heap-based, inspired by my solution to yesterday's
// problem "The Number Of The Smallest Unoccupied Chair".
// Complexity: runtime O(nlogn), space O(n).

static int cmp(const void *const a, const void *const b)
{
    return (*(const int *const *)a)[0] - (*(const int *const *)b)[0];
}

int minGroups(int *const *const ivs, const int n, const int *const _)
{
    qsort(ivs, n, sizeof ivs[0], cmp); // sort intervals by start times

    int heap[100000], heap_len = 0, ret = 0;
    for (int i = 0; i < n; ++i) {
        const int *const iv = ivs[i];
        // pop end times lower than this interval's start from the heap
        while (heap_len > 0 && iv[0] > heap[0]) {
            --heap_len;
            int ni = 0;
            while (true) {
                const int li = 2 * ni + 1, ri = li + 1,
                          lv = li <= heap_len ? heap[li] : INT_MAX,
                          rv = ri <= heap_len ? heap[ri] : INT_MAX;
                if (heap[heap_len] < lv && heap[heap_len] < rv)
                    break;
                const int ni2 = lv < rv ? li : ri;
                heap[ni] = heap[ni2];
                ni = ni2;
            }
            heap[ni] = heap[heap_len];
        }

        // push this interval's end time into the heap
        int ni = heap_len++;
        if (heap_len > ret) // current overlap is the size of the heap, so max
            ret = heap_len; // overlap is the max size of the heap
        while (ni > 0) {
            const int pi = (ni - 1) / 2;
            if (iv[1] >= heap[pi])
                break;
            heap[ni] = heap[pi];
            ni = pi;
        }
        heap[ni] = iv[1];
    }

    return ret;
}
