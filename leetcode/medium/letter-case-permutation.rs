// https://leetcode.com/problems/letter-case-permutation
//
// Complexity: runtime O(n*(2^n)), space O(n).

impl Solution {
    pub fn letter_case_permutation(s: String) -> Vec<String> {
        fn dfs(svec: &mut Vec<u8>, results: &mut Vec<String>, i: usize) {
            if i >= svec.len() {
                // svec only contains ascii, so this is fine
                results.push(unsafe { String::from_utf8_unchecked(svec.clone()) });
                return;
            }

            dfs(svec, results, i + 1);
            if svec[i].is_ascii_alphabetic() {
                if svec[i].is_ascii_uppercase() {
                    svec[i].make_ascii_lowercase()
                } else {
                    svec[i].make_ascii_uppercase()
                }

                dfs(svec, results, i + 1);
                // NOTE: no explicit need to backtrack here, as the case-toggling logic will
                // re-toggle any chars left as upper-case back to lower-case once the nested
                // recursive calls get a chance to finish
            }
        }

        let mut svec = s.into_bytes(); // again, only ascii, so this is fine
        let mut results = Vec::new();
        dfs(&mut svec, &mut results, 0);
        results
    }
}

// Alternative Solution: Toggle case via bit manipulation.
//
// For ASCII alphabetic letters, the bit at index 5 (0-indexed) is set if the character is
// upper-case; unset otherwise.
//
// Complexity: runtime O(n*(2^n)), space O(n).

impl Solution {
    pub fn letter_case_permutation(s: String) -> Vec<String> {
        fn dfs(svec: &mut Vec<u8>, results: &mut Vec<String>, i: usize) {
            if i >= svec.len() {
                results.push(unsafe { String::from_utf8_unchecked(svec.clone()) });
            } else {
                dfs(svec, results, i + 1);
                if svec[i].is_ascii_alphabetic() {
                    svec[i] ^= 0b10_0000; // equivalent to svec[i] ^= 1 << 5
                    dfs(svec, results, i + 1);
                }
            }
        }

        let mut svec = s.into_bytes();
        let mut results = Vec::new();
        dfs(&mut svec, &mut results, 0);
        results
    }
}
