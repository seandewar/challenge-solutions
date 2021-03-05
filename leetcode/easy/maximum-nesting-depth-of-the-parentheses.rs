// https://leetcode.com/problems/maximum-nesting-depth-of-the-parentheses
//
// Complexity: runtime O(n), space O(1).

impl Solution {
    pub fn max_depth(s: String) -> i32 {
        s.chars()
            .scan(0, |depth, c| {
                match c {
                    '(' => *depth += 1,
                    ')' => *depth -= 1,
                    _ => (),
                }
                Some(*depth)
            })
            .max()
            .unwrap()
    }
}
