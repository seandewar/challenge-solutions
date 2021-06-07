// https://leetcode.com/problems/min-cost-climbing-stairs
//
// Complexity: runtime O(n), space O(1).

impl Solution {
    pub fn min_cost_climbing_stairs(cost: Vec<i32>) -> i32 {
        cost.iter()
            .chain(std::iter::once(&0))
            .fold((0, 0), |(a, b), x| (x + a.min(b), a))
            .0
    }
}
