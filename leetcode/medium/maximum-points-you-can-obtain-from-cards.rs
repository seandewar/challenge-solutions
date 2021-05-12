// https://leetcode.com/problems/maximum-points-you-can-obtain-from-cards
//
// This can be optimized to O(n) by re-using and updating the previous window sum rather than
// re-calculating the entire thing, but I'm busy. :^)
//
// Complexity: runtime O(n^2), space O(1).

impl Solution {
    pub fn max_score(cards: Vec<i32>, k: i32) -> i32 {
        let sum: i32 = cards.iter().sum();
        let win_len = cards.len() - k as usize;
        if win_len == 0 {
            sum
        } else {
            cards
                .windows(win_len)
                .map(|w| sum - w.iter().sum::<i32>())
                .max()
                .unwrap()
        }
    }
}
