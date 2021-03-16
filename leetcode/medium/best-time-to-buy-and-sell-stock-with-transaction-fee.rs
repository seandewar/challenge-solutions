// https://leetcode.com/problems/best-time-to-buy-and-sell-stock-with-transaction-fee
//
// Greedy solution.
// Complexity: runtime O(n), space O(1).

impl Solution {
    pub fn max_profit(prices: Vec<i32>, fee: i32) -> i32 {
        prices[1..]
            .iter()
            .fold((prices[0], 0), |(min, acc), &p| {
                if p > min + fee {
                    (p - fee, acc + p - min - fee)
                } else {
                    (p.min(min), acc)
                }
            })
            .1
    }
}
