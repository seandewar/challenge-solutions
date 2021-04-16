// https://leetcode.com/problems/remove-all-adjacent-duplicates-in-string-ii
//
// Stack-based.
// Complexity: runtime O(n), space O(n).

impl Solution {
    pub fn remove_duplicates(s: String, k: i32) -> String {
        s.chars()
            .fold(vec![], |mut s, a| {
                match s.last_mut() {
                    Some((b, n)) if a == *b => {
                        *n += 1;
                        if *n == k {
                            s.pop();
                        }
                    }
                    _ => s.push((a, 1)),
                }
                s
            })
            .into_iter()
            .flat_map(|(c, n)| std::iter::repeat(c).take(n as _))
            .collect()
    }
}

// Alternative Solution: Naive; surprisingly, this passes.
// Complexity: runtime O(n^2), space O(1).

impl Solution {
    pub fn remove_duplicates(s: String, k: i32) -> String {
        let (mut s, k) = (s.into_bytes(), k as usize);
        let mut len = 0;
        while len != s.len() {
            len = s.len();
            for i in k - 1..len {
                if s[i - k + 1..i].iter().all(|&c| c == s[i]) {
                    s[i - k + 1..=i].iter_mut().for_each(|c| *c = 0);
                }
            }
            s.retain(|&c| c != 0);
        }

        unsafe { String::from_utf8_unchecked(s) }
    }
}
