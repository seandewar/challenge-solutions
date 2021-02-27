// https://leetcode.com/problems/split-a-string-in-balanced-strings
//
// Complexity: runtime O(n), space O(1).

impl Solution {
    pub fn balanced_string_split(s: String) -> i32 {
        let (mut res, mut left_char, mut req) = (0, '\0', 0);
        for c in s.chars() {
            if c == left_char {
                // expand the left-side of sub-string
                req += 1;
            } else if req > 0 {
                // not the same char as on the left side; assume the other side and expand the
                // right-side of sub-string, until we equal the size of the left side
                req -= 1;
                if (req == 0) {
                    res += 1;
                }
            } else {
                // not the same char as our left side, and we weren't expecting it; start a new
                // sub-string here, with this char as what's on the left side
                left_char = c;
                req = 1;
            }
        }

        res
    }
}

// Alternative Solution: Condensed using Iterator::fold().
// Complexity: runtime O(n), space O(1).

impl Solution {
    pub fn balanced_string_split(s: String) -> i32 {
        s.chars()
            .fold((0, '\0', 0), |(res, left_c, req), c| {
                if c == left_c {
                    (res, left_c, req + 1)
                } else if req > 0 {
                    (res + if req == 1 { 1 } else { 0 }, left_c, req - 1)
                } else {
                    (res, c, 1)
                }
            })
            .0
    }
}
