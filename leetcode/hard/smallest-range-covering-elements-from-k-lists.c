// https://leetcode.com/problems/smallest-range-covering-elements-from-k-lists
//
// Window-based solution; merge the lists, keeping track of the list where a
// number is from, and sort. Iterate through the merged list using a window (min
// value is always the leftmost position of the window, and max the rightmost),
// and expand it if the window doesn't cover values from all lists, shrink
// otherwise; update the best range while doing this.
//
// The initial merging of the lists can be replaced with a min heap that's
// pushed to when logically advancing a list, but after two heap solutions in
// the last two days ("Divide Intervals Into Minimum Number Of Groups" and "The
// Number Of The Smallest Unoccupied Chair"), I CBA. :-)
//
// Complexity: runtime O(nlogn), space O(n), where n == total nums in all lists.

struct Num {
    int v, i;
};

static int cmp(const void *const a, const void *const b)
{
    return ((const struct Num *)a)->v - ((const struct Num *)b)->v;
}

int *smallestRange(const int *const *const nums, const int n,
                   const int *const ms, int *const retn)
{
    // Possible to avoid this merging of the arrays if we use a heap that we
    // only push to when advancing the arrays, but I'm sick of heaps (in C).
    int alln = 0;
    struct Num all_nums[50 * 3500];
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < ms[i]; ++j)
            all_nums[alln++] = (const struct Num){nums[i][j], i};
    qsort(all_nums, alln, sizeof all_nums[0], cmp);

    int win_counts[3501] = {0}, in_win_count = 0, wini = 0,
        winn = 0, *const best = malloc(2 * sizeof best[0]);
    best[0] = 0;
    best[1] = INT_MAX;
    while (wini + winn <= alln) {
        if (in_win_count < n) {
            const int i = all_nums[wini + winn++].i;
            if (++win_counts[i] == 1)
                ++in_win_count;
            continue;
        }
        if (all_nums[wini + winn - 1].v - all_nums[wini].v
            < best[1] - best[0]) {
            best[0] = all_nums[wini].v;
            best[1] = all_nums[wini + winn - 1].v;
        }
        --winn;
        const int i = all_nums[wini++].i;
        if (--win_counts[i] == 0)
            --in_win_count;
    }

    *retn = 2;
    return best;
}

// Alternative Solution: straightforward method (my initial solution).
//
// This approach starts at the beginning of each list, finds the range required
// to fit the values selected in all lists (via finding the min and max values),
// then advances the position of the list containing the smallest value AFTER
// those selected. After we reach the end of all lists, we return the smallest
// range seen.
//
// It's probably possible to speed up the inner loop that looks for the list
// with the next smallest value to advance to by using a min heap.
//
// Complexity: runtime O(nk), space O(1), where n == total nums in all lists
// [technically space is O(k) if we stored the current indices of each list in a
// different array, but we instead modify the input pointers].

int *smallestRange(const int **const nums, const int n, int *const ms,
                   int *const retn)
{
    int *const best = malloc(2 * sizeof best[0]);
    best[0] = 0;
    best[1] = INT_MAX;

    while (true) {
        int min = INT_MAX, max = INT_MIN, next_min = INT_MAX, next_i = -1;
        for (int i = 0; i < n; ++i) {
            const int v = nums[i][0];
            if (v < min)
                min = v;
            if (v > max)
                max = v;
            if (ms[i] < 2)
                continue;

            const int next = nums[i][1];
            if (next < next_min) {
                next_min = next;
                next_i = i;
            }
        }
        if (max - min < best[1] - best[0]) {
            best[0] = min;
            best[1] = max;
        }
        if (next_i == -1)
            break;

        ++nums[next_i];
        --ms[next_i];
    }

    *retn = 2;
    return best;
}
