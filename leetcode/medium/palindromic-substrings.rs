// https://leetcode.com/problems/palindromic-substrings
//
// Expand around centres.
// Complexity: runtime O(n^2), space O(1).

impl Solution {
    pub fn count_substrings(s: String) -> i32 {
        let s = s.into_bytes();
        let count = |i, j| {
            ((0..=i).rev())
                .zip(j..s.len())
                .try_fold(0, |a, (i, j)| if s[i] == s[j] { Ok(a + 1) } else { Err(a) })
                .unwrap_or_else(|a| a)
        };

        (0..s.len()).map(|i| count(i, i) + count(i, i + 1)).sum()
    }
}

// Storing slices to expand around centres starting from palindromes of lengths 1 & 2.
// Complexity: runtime O(n^2), space O(n).

impl Solution {
    pub fn count_substrings(s: String) -> i32 {
        let s = s.into_bytes();
        let mut palins: Vec<_> = s
            .windows(1)
            .chain(s.windows(2).filter(|w| w[0] == w[1]))
            .collect();
        let mut res = palins.len();

        while !palins.is_empty() {
            palins = palins
                .into_iter()
                .filter_map(|p| Self::extend_slice(&s, p))
                .filter(|p| p[0] == p[p.len() - 1])
                .collect();
            res += palins.len();
        }

        res as _
    }

    fn extend_slice<'a, T>(outer: &'a [T], inner: &'a [T]) -> Option<&'a [T]> {
        let (outp, inp) = (outer.as_ptr() as usize, inner.as_ptr() as usize);
        if outp != inp && outp + outer.len() != inp + inner.len() {
            let newi = inp - outp - 1;
            Some(&outer[newi..newi + inner.len() + 2])
        } else {
            None
        }
    }
}
