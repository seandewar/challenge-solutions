// https://leetcode.com/problems/evaluate-reverse-polish-notation
//
// Complexity: runtime O(n), space O(n).

impl Solution {
    pub fn eval_rpn(tokens: Vec<String>) -> i32 {
        let mut s = vec![];
        for t in &tokens {
            if let Ok(x) = t.parse() {
                s.push(x);
            } else {
                let (y, x) = (s.pop().unwrap(), s.pop().unwrap());
                s.push(match t.as_str() {
                    "+" => x + y,
                    "-" => x - y,
                    "*" => x * y,
                    "/" => x / y,
                    _ => unreachable!(),
                });
            }
        }
        s.last().copied().unwrap_or(0)
    }
}
