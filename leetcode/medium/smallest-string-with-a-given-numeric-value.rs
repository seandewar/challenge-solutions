// https://leetcode.com/problems/smallest-string-with-a-given-numeric-value
//
// k_rem is, for each character *c in the loop, the required numeric character value *c should have
// to make res the lexicographically smallest string to return.
//
// k_rem is calculated from the observation that we can create the lexicographically smallest
// string by maximizing the numeric sum of character values towards the end of res, while
// minimizing the sum of values towards the beginning of res. To do this, we can insert repeating
// "z"s towards the end and a repeating "a"s towards the beginning of res as necessary, while
// inserting a single character between the repeated parts to cover any difference between the
// numeric sums of the repeating parts and k.
//
// If k_rem is in the range [1, 26], we can simply insert the character with the corresponding
// numeric value by adding it as an offset to the ASCII value of "a", minus 1. If k_rem is greater
// than 26, we insert "z". If k_rem is less than 1`, we insert "a".
//
// Complexity: runtime O(n), space O(1) [O(n) including output].

impl Solution {
    pub fn get_smallest_string(n: i32, k: i32) -> String {
        let mut res = vec![0; n as _];
        for (i, c) in res.iter_mut().enumerate().rev() {
            let k_rem = k - (i as i32 + 26 * (n - i as i32 - 1));
            *c = b'a' + (k_rem - 1).min(25).max(0) as u8;
        }
        String::from_utf8(res).unwrap()
    }
}


// Alternative Solution: Optimized slightly.
//
// Rather than calculate k_rem each time, we can rewrite things to simply adjust k to the new value
// of k_rem on each iteration of the loop, which also allows us to get rid of the i and the
// enumerate() fluff.
//
// res is a Vec<u8> representing an ASCII string. UTF-8 is backward-compatible with ASCII for
// character bytes with values in the range [0, 128], so we can use String::from_utf8_unchecked()
// when creating our returned String.
//
// Do note that String::from_utf8_unchecked() is considered unsafe, which is why we need to
// surround it in an unsafe block. This is because Strings in Rust are always valid UTF-8 when
// manipulated via safe methods; however, because from_utf8_unchecked() does not check for valid
// UTF-8, it is marked as unsafe, as we can potentially violate this guarantee and give it invalid
// UTF-8, resulting in undefined behaviour.
//
// Complexity: runtime O(n), space O(1) [O(n) including output].

impl Solution {
    pub fn get_smallest_string(n: i32, mut k: i32) -> String {
        let mut res = vec![0; n as _];
        k -= n - 1;
        for c in res.iter_mut().rev() {
            *c = b'a' + (k - 1).max(0).min(25) as u8;
            k -= (*c - b'a') as i32;
        }
        unsafe { String::from_utf8_unchecked(res) }
    }
}
