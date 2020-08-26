// https://leetcode.com/problems/minimum-cost-for-tickets/
//
// Dynamic programming, bottom-up, iterative solution.
// Complexity: runtime O(1), space O(1).

int mincostTickets(const int* const restrict days, const int daysSize,
                   const int* const restrict costs, const int costsSize)
{
    const int firstTravelDay = days[0];
    const int lastTravelDay = days[daysSize - 1];

    int cache[366]; // could use a naughty stack-allocated VLA here :)
    cache[firstTravelDay - 1] = 0;

    for (int i = firstTravelDay, dayIdx = 0; i <= lastTravelDay; ++i) {
        // make sure we only act on days that we are required to travel on
        if (i != days[dayIdx]) {
            cache[i] = cache[i - 1];
            continue;
        }

        // 1-day ticket
        cache[i] = cache[i - 1] + costs[0];

        // 7-day ticket
        int j = i >= 7 ? i - 7 : 0;

        if (cache[j] + costs[1] < cache[i]) {
            cache[i] = cache[j] + costs[1];
        }

        // 30-day ticket
        j = i >= 30 ? i - 30 : 0;

        if (cache[j] + costs[2] < cache[i]) {
            cache[i] = cache[j] + costs[2];
        }

        ++dayIdx;
    }

    return cache[lastTravelDay];
}
