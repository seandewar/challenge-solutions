// https://leetcode.com/problems/largest-substring-between-two-equal-characters
//
// Using hash map. Can also be done using an array.
// Complexity: runtime O(n), space O(26) == O(1).

impl Solution {
    pub fn max_length_between_equal_characters(s: String) -> i32 {
        let mut map = std::collections::HashMap::new();
        s.chars().enumerate().for_each(|(i, c)| {
            map.entry(c).or_insert((i as i32 + 1, 0)).1 = i as i32 - 1;
        });
        map.into_iter()
            .map(|(_, (i, j))| j - i + 1)
            .max()
            .unwrap_or(-1)
    }
}

// Alternative Solution: Iterators.
// Complexity: runtime O(26 * 2 * n) == O(n), space O(1).

impl Solution {
    pub fn max_length_between_equal_characters(s: String) -> i32 {
        let s = s.as_bytes();
        (b'a'..=b'z')
            .map(|c| {
                let i = (0..s.len()).find(|&i| s[i] == c).unwrap_or(0) + 1;
                let j = (0..s.len()).rev().find(|&i| s[i] == c).unwrap_or(0) - 1;
                j as i32 - i as i32 + 1
            })
            .max()
            .unwrap()
    }
}
