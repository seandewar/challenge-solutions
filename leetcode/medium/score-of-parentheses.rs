// https://leetcode.com/problems/score-of-parentheses
//
// Complexity: runtime O(n), space O(n).

impl Solution {
    pub fn score_of_parentheses(s: String) -> i32 {
        let mut stack = vec![0];
        for c in s.chars() {
            match c {
                '(' => stack.push(0),
                ')' => {
                    let top = stack.pop().unwrap();
                    *stack.last_mut().unwrap() += (2 * top).max(1);
                }
                _ => unreachable!(),
            }
        }
        stack[0]
    }
}
