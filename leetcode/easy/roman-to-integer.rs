// https://leetcode.com/problems/roman-to-integer
//
// Complexity: runtime O(n), space O(1).

impl Solution {
    pub fn roman_to_int(s: String) -> i32 {
        s.chars().rfold(0, |acc, c| {
            acc + match c {
                'I' if acc > 4 => -1,
                'I' => 1,
                'V' => 5,
                'X' if acc > 49 => -10,
                'X' => 10,
                'L' => 50,
                'C' if acc > 499 => -100,
                'C' => 100,
                'D' => 500,
                'M' => 1000,
                _ => 0,
            }
        })
    }
}
