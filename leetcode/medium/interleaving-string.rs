// https://leetcode.com/problems/interleaving-string
//
// Complexity: runtime O(m*n), space O(m*n).

impl Solution {
    pub fn is_interleave(s1: String, s2: String, s3: String) -> bool {
        let mut mem = [[None; 101]; 101];
        Self::check(&s1, &s2, &s3, &mut mem)
    }

    fn check(s1: &str, s2: &str, s3: &str, mem: &mut [[Option<bool>; 101]; 101]) -> bool {
        let (i, j) = (s1.len(), s2.len());
        if mem[i][j].is_none() {
            let valid = match (s1.chars().next(), s2.chars().next(), s3.chars().next()) {
                (Some(a), Some(b), Some(c)) if a == c || b == c => {
                    a == c && Self::check(&s1[1..], s2, &s3[1..], mem)
                        || b == c && Self::check(s1, &s2[1..], &s3[1..], mem)
                }
                (_, None, _) => s1 == s3,
                (None, _, _) => s2 == s3,
                _ => false,
            };
            mem[i][j] = Some(valid);
        }
        mem[i][j].unwrap()
    }
}
