// https://leetcode.com/problems/goal-parser-interpretation
//
// Complexity: runtime O(n), space O(1) [O(n) including output].

impl Solution {
    pub fn interpret(command: String) -> String {
        command.replace("()", "o").replace("(al)", "al")
    }
}

// Alternative Solution: Manual implementation.
// Complexity: runtime O(n), space O(1) [O(n) including output].

impl Solution {
    pub fn interpret(command: String) -> String {
        let mut res = String::new();
        let mut slice = &command[..];

        while !slice.is_empty() {
            if slice.starts_with("()") {
                slice = &slice[2..];
                res.push('o');
            } else if slice.starts_with("(al)") {
                slice = &slice[4..];
                res.push_str("al");
            } else {
                res.push(slice.chars().next().unwrap());
                slice = &slice[1..];
            }
        }

        res
    }
}
