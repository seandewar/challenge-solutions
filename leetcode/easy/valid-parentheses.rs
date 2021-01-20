// https://leetcode.com/problems/valid-parentheses
//
// Complexity: runtime O(n), space O(n).

impl Solution {
    pub fn is_valid(s: String) -> bool {
        let mut paren_stack = Vec::new();
        for c in s.chars() {
            paren_stack.push(match c {
                '(' => ')',
                '[' => ']',
                '{' => '}',
                _ if paren_stack.pop().map_or(false, |x| c == x) => continue,
                _ => return false,
            });
        }

        paren_stack.is_empty()
    }
}
