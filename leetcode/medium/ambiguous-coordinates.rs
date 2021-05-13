// https://leetcode.com/problems/ambiguous-coordinates
//
// Complexity: runtime O(n^4), space O(1) ignoring output.

impl Solution {
    pub fn ambiguous_coordinates(s: String) -> Vec<String> {
        let s = s.into_bytes(); // s is ASCII, so we may work with it as bytes
        let s = &s[1..s.len() - 1]; // ignore the parenthesis around the coords

        (1..s.len()).fold(vec![], |mut acc, comma_i| {
            let (x, y) = s.split_at(comma_i);
            (0..x.len())
                .filter_map(|dot_i| Self::make_coord(x, dot_i))
                .for_each(|x| {
                    (0..y.len())
                        .filter_map(|dot_i| Self::make_coord(y, dot_i))
                        .for_each(|y| acc.push(format!("({}, {})", x, y)))
                });

            acc
        })
    }

    fn make_coord(s: &[u8], dot_i: usize) -> Option<String> {
        if (s.len() == 1 || dot_i == 1 || s[0] != b'0') && (dot_i == 0 || s[s.len() - 1] != b'0') {
            let (l, r) = s.split_at(dot_i);
            let l = unsafe { String::from_utf8_unchecked(l.into()) }; // these are safe, as s is
            let r = unsafe { String::from_utf8_unchecked(r.into()) }; // always valid ASCII

            Some(format!("{}{}{}", l, if l.is_empty() { "" } else { "." }, r))
        } else {
            None
        }
    }
}
