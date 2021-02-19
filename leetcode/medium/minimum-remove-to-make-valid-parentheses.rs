// https://leetcode.com/problems/minimum-remove-to-make-valid-parentheses
//
// Complexity: runtime O(n), space O(n).

impl Solution {
    pub fn min_remove_to_make_valid(s: String) -> String {
        let mut valid_parens = std::collections::HashSet::new();
        let mut open_parens = Vec::new();
        for (i, c) in s.chars().enumerate() {
            match c {
                '(' => open_parens.push(i),
                ')' => {
                    if let Some(open_idx) = open_parens.pop() {
                        valid_parens.insert(open_idx);
                        valid_parens.insert(i);
                    }
                }
                _ => continue,
            }
        }
        s.chars()
            .enumerate()
            .filter(|&(i, c)| (c != '(' && c != ')') || valid_parens.contains(&i))
            .map(|(_, c)| c)
            .collect()
    }
}
