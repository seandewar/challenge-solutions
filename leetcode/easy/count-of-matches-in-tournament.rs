// https://leetcode.com/problems/count-of-matches-in-tournament
//
// There are n teams.
// n - 1 teams need to be eliminated for us to have a champion, requiring n - 1 matches.
// Therefore, there will be n - 1 matches in total.
//
// Complexity: runtime O(1), space O(1).

impl Solution {
    pub fn number_of_matches(n: i32) -> i32 {
        n - 1
    }
}

// Alternative Solution: Iterative.
// Complexity: runtime O(logn), space O(1).

impl Solution {
    pub fn number_of_matches(mut n: i32) -> i32 {
        let mut res = 0;
        while n > 1 {
            res += n / 2;
            n = (n / 2) + n % 2;
        }

        res
    }
}
