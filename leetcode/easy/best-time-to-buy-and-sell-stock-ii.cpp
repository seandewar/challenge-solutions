// https://leetcode.com/problems/best-time-to-buy-and-sell-stock-ii/
//
// Simple one-pass solution. We always track the previous price, whatever it
// was. If the current price is bigger, we simulate selling the stock at the
// current price as if we had bought it at the previous price by adding the
// difference to our max profit.
//
// By always tracking the previous price, it doesn't matter if we simulate
// selling early when a bigger sale profit is available later. This is because
// we are definitely not subtracting from our profit when buying any stocks,
// so the value will represent the max profit that we can possibly make if we
// had bought and sold stocks at the most optimal times. In other words, by
// adding the max profits for every particular sub-section of the array where
// the price in stocks rise, we get the max profit for the whole array.
//
// There are other solutions, but this was the first that came to mind and is
// by far the simplest. A similar solution is used for the original "Best Time
// to Buy and Sell Stock 1" problem.
//
// Complexity: runtime O(n), space O(1).

class Solution {
public:
    int maxProfit(vector<int>& prices)
    {
        int maxProfit{};

        for (vector<int>::size_type i = 1; i < size(prices); ++i)
            maxProfit += max(0, prices[i] - prices[i - 1]);

        return maxProfit;
    }
};
