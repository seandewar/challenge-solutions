// https://leetcode.com/problems/kids-with-the-greatest-number-of-candies
//
// Complexity: runtime O(TODO), space O(TODO).

impl Solution {
    pub fn kids_with_candies(candies: Vec<i32>, extra_candies: i32) -> Vec<bool> {
        let max = *candies.iter().max().unwrap();
        candies
            .into_iter()
            .map(|x| x + extra_candies >= max)
            .collect()
    }
}
