// https://leetcode.com/problems/best-time-to-buy-and-sell-stock.c
//
// Complexity: runtime O(n), space O(1).

int maxProfit(const int *const prices, const int n)
{
    int ret = 0, max = prices[0], min = prices[0];
    for (int i = 1; i < n; ++i) {
        if (prices[i] < min) {
            max = min = prices[i];
        } else if (prices[i] > max) {
            max = prices[i];
            const int diff = max - min;
            ret = diff > ret ? diff : ret;
        }
    }
    return ret;
}
