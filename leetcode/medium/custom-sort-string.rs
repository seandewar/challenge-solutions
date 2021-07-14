// https://leetcode.com/problems/custom-sort-string
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
