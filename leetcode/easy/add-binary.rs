// https://leetcode.com/problems/add-binary/
//
// Complexity: runtime O(n+m), space O(n+m) [due to intermediate rev_result, but you can ignore
// this and say O(1) if you want; it's more of a rust thing].

impl Solution {
    pub fn add_binary(a: String, b: String) -> String {
        let mut rev_result = Vec::new();
        let mut carry_set = false;

        let mut a_iter = a.chars().rev();
        let mut b_iter = b.chars().rev();

        loop {
            let a_next = a_iter.next();
            let b_next = b_iter.next();

            if a_next.is_none() && b_next.is_none() && !carry_set {
                break;
            }

            let a_set = a_next.unwrap_or('0') == '1';
            let b_set = b_next.unwrap_or('0') == '1';

            let result_set = a_set ^ b_set ^ carry_set;
            carry_set = (a_set & b_set) | ((a_set | b_set) & carry_set);

            rev_result.push(if result_set { '1' } else { '0' });
        }

        rev_result.iter().rev().collect()
    }
}
