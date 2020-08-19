// https://leetcode.com/problems/goat-latin/
//
// Complexity: runtime O(w^2 + l*w), space O(w^2 + l*w) [where w = num words in input, l = num
// chars in a word (upper-bound)].

impl Solution {
    pub fn to_goat_latin(s: String) -> String {
        let mut words = s
            .split_ascii_whitespace()
            .map(|s| s.to_owned())
            .collect::<Vec<_>>();

        for (i, w) in words.iter_mut().enumerate() {
            match w.chars().next().unwrap().to_ascii_lowercase() {
                'a' | 'e' | 'i' | 'o' | 'u' => {}
                _ => {
                    let c = w.remove(0);
                    w.push(c);
                }
            }

            w.push_str("ma");
            w.push_str(&"a".repeat(i + 1));
        }

        words.join(" ")
    }
}
