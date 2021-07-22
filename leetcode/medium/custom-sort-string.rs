// https://leetcode.com/problems/custom-sort-string
//
// str is valid ASCII, so we can sort its bytes.
// Here, we move out str's bytes, sort them, then move them back into a String.
//
// Complexity: runtime O(n*logn), space O(logn).

impl Solution {
    pub fn custom_sort_string(order: String, str: String) -> String {
        let order_map = order.chars().enumerate().fold([0; 26], |mut acc, (i, c)| {
            acc[c as usize - 'a' as usize] = i;
            acc
        });
        let mut s = str.into_bytes();
        s.sort_unstable_by_key(|&c| order_map[c as usize - 'a' as usize]);
        String::from_utf8(s).unwrap()
    }
}

// Alternative Solution: Sort str's bytes directly (no moves; in-place).
// Complexity: runtime O(n*logn), space O(logn).

impl Solution {
    pub fn custom_sort_string(order: String, mut str: String) -> String {
        let order_map = order.chars().enumerate().fold([0; 26], |mut acc, (i, c)| {
            acc[c as usize - 'a' as usize] = i;
            acc
        });
        let mut s = unsafe { str.as_mut_vec() };
        s.sort_unstable_by_key(|&c| order_map[c as usize - 'a' as usize]);
        str
    }
}
