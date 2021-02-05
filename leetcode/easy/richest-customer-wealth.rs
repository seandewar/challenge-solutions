// https://leetcode.com/problems/richest-customer-wealth
//
// Complexity: runtime O(n*m), space O(1).

impl Solution {
    pub fn maximum_wealth(accounts: Vec<Vec<i32>>) -> i32 {
        accounts
            .into_iter()
            .map(|b| b.into_iter().sum())
            .max()
            .unwrap()
    }
}
