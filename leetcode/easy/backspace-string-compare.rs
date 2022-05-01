// https://leetcode.com/problems/backspace-string-compare
// No stack; constant space.
// Complexity: runtime O(n), space O(1).

impl Solution {
    pub fn backspace_compare(s: String, t: String) -> bool {
        let (mut s_it, mut t_it) = (s.chars().rev().peekable(), t.chars().rev().peekable());
        let (mut s_skip, mut t_skip) = (0, 0);
        loop {
            match (s_it.peek(), t_it.peek()) {
                (Some('#'), _) => {
                    s_skip += 1;
                    s_it.next();
                }
                (Some(_), _) if s_skip > 0 => {
                    s_skip -= 1;
                    s_it.next();
                }
                (_, Some('#')) => {
                    t_skip += 1;
                    t_it.next();
                }
                (_, Some(_)) if t_skip > 0 => {
                    t_skip -= 1;
                    t_it.next();
                }
                (Some(a), Some(b)) if a == b => {
                    s_it.next();
                    t_it.next();
                }
                (None, None) => return true,
                _ => return false,
            }
        }
    }
}
