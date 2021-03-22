// https://leetcode.com/problems/unique-morse-code-words
//
// Complexity: runtime O(n), space O(n). [where n == total chars in all words]

impl Solution {
    pub fn unique_morse_representations(words: Vec<String>) -> i32 {
        static MORSE: [&str; 26] = [
            ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", "-.-", ".-..",
            "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-", "..-", "...-", ".--", "-..-",
            "-.--", "--..",
        ];

        words
            .into_iter()
            .map(|w| {
                w.chars()
                    .map(|c| MORSE[c as usize - 'a' as usize])
                    .collect::<String>()
            })
            .collect::<std::collections::HashSet<_>>()
            .len() as _
    }
}
