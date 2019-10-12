// https://leetcode.com/problems/best-time-to-buy-and-sell-stock/
//
// Simply keeps track of the current max profit & the current lowest buy. If
// the current value isn't a new lowest buy, see how much profit we'd make if
// we sold at that day, and adjust the current max profit to this value if it's
// larger.
//
// Complexity: runtime O(n), space O(1).

class Solution {
public:
    int maxProfit(vector<int>& prices)
    {
        int maxProfit = 0, lowBuy = numeric_limits<int>::max();

        for (const int price : prices) {
            if (price < lowBuy) {
                lowBuy = price;
            }
            else {
                maxProfit = max(maxProfit, price - lowBuy);
            }
        }

        return maxProfit;
    }
};

// Solution can also be written as:
//
// for (const int price : prices) {
//     lowBuy = min(lowBuy, price);
//     maxProfit = max(maxProfit, price - lowBuy);
// }
//
// But this alternative will still calculate maxProfit even if lowBuy was just
// changed, which isn't necessary. It's an insignificant slowdown though, and
// this alternative is more readable.
