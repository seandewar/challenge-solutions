// https://leetcode.com/problems/determine-if-two-strings-are-close
//
// Complexity: runtime O(n), space O(1).

impl Solution {
    pub fn close_strings(word1: String, word2: String) -> bool {
        if word1.len() != word2.len() {
            return false;
        }

        // frequency map of characters in both words. This is all we need, as because of operation
        // 1 allowing swaps, we can get chars in the positions we need to make the other word after
        // operation 2 is performed
        let (mut freqs1, mut freqs2) = ([0; 26], [0; 26]);
        for (c1, c2) in word1.chars().zip(word2.chars()) {
            freqs1[c1 as usize - 'a' as usize] += 1;
            freqs2[c2 as usize - 'a' as usize] += 1;
        }

        // differing set of chars in one word means they can't be close
        for (&f1, &f2) in freqs1.iter().zip(freqs2.iter()) {
            match (f1, f2) {
                (f1, 0) if f1 > 0 => return false,
                (0, f2) if f2 > 0 => return false,
                _ => continue,
            }
        }

        // for operation 2, sort the frequency maps. If they are equal, it will be possible to
        // change occurrences of one char to another in order to create the other word
        freqs1.sort_unstable();
        freqs2.sort_unstable();
        freqs1 == freqs2
    }
}
