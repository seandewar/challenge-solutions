// https://leetcode.com/problems/find-and-replace-pattern
//
// Complexity: runtime O(n*p), space O(1) ignoring output. [where n == words.len(), p ==
// pattern.len() (also equal to words[i].len())]

impl Solution {
    pub fn find_and_replace_pattern(words: Vec<String>, pattern: String) -> Vec<String> {
        words
            .into_iter()
            .filter(|w| {
                let mut pat_map = vec!['\0'; 26];
                w.chars()
                    .zip(pattern.chars().map(|p| p as usize - 'a' as usize))
                    .all(|(c, i)| {
                        if pat_map[i] == '\0' {
                            pat_map[i] = c;
                            pat_map.iter().filter(|&&c| c == pat_map[i]).count() == 1
                        } else {
                            c == pat_map[i]
                        }
                    })
            })
            .collect()
    }
}
