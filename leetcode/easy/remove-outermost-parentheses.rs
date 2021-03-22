// https://leetcode.com/problems/remove-outermost-parentheses
//
// Complexity: runtime O(n), space O(1) [O(n) if including output].

impl Solution {
    pub fn remove_outer_parentheses(s: String) -> String {
        s.chars()
            .fold(
                (String::with_capacity(s.len()), 0),
                |(mut res, mut depth), c| {
                    match c {
                        '(' => {
                            depth += 1;
                            if depth > 1 {
                                res.push(c);
                            }
                        }
                        ')' => {
                            depth -= 1;
                            if depth > 0 {
                                res.push(c);
                            }
                        }
                        _ => unreachable!(),
                    }

                    (res, depth)
                },
            )
            .0
    }
}
