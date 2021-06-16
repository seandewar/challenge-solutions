// https://leetcode.com/problems/generate-parentheses
//
// Complexity: see C++ solution.

impl Solution {
    pub fn generate_parenthesis(n: i32) -> Vec<String> {
        let (mut res, mut tmp) = (vec![], String::with_capacity(n as usize * 2));
        Self::generate(&mut res, &mut tmp, n as _, 0);
        res
    }

    fn generate(out: &mut Vec<String>, cur: &mut String, n: u32, open: u32) {
        if (n, open) == (0, 0) {
            out.push(cur.clone());
        } else {
            let mut backtrack = |c, n, open, cond| {
                if cond {
                    cur.push(c);
                    Self::generate(out, cur, n, open);
                    cur.pop();
                }
            };
            backtrack('(', n - 1, open + 1, n > 0);
            backtrack(')', n, open - 1, open > 0);
        }
    }
}
