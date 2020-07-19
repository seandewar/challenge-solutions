// https://leetcode.com/problems/add-binary/
//
// Zipped and extended iterator solution.
// See code comments for details.
//
// Complexity: runtime O(n+m), space O(n+m) [due to intermediate rev_result, but you can ignore
// this and say O(1) if you want; it's more of a rust thing].

impl Solution {
    pub fn add_binary(mut a: String, mut b: String) -> String {
        // when creating the bit_set_iter iterator, we'll be zipping both a & b's iterators
        // together; these zipped iterators will return None on next() if just one of a or b's
        // iterators returns None on next(). this means we'll need to make a distinction between
        // the larger and smaller of a & b, so that we can extend the smaller string's iterator to
        // repeat 0s to the size of the bigger one so that they may be zipped together such that
        // the zipped iterator will not return None from next() until both a & b's iterators are
        // exhausted. we'll have "a" refer to the biggest string, and "b" refer to the smallest.
        if b.len() > a.len() {
            std::mem::swap(&mut a, &mut b);
        }

        let mut bit_set_iter = b
            .chars()
            .rev()
            .map(|c| c == '1')
            .chain(std::iter::repeat(false)) // extend b (smallest string) with 0s
            .zip(a.chars().rev().map(|c| c == '1')); // zipped iter ends with a (biggest string)

        let mut rev_result = Vec::new();
        let mut carry_set = false;

        for (a_set, b_set) in bit_set_iter {
            let result_set = a_set ^ b_set ^ carry_set;
            rev_result.push(if result_set { '1' } else { '0' });
            carry_set = (a_set & b_set) | ((a_set | b_set) & carry_set);
        }

        if carry_set {
            rev_result.push('1');
        }

        rev_result.iter().rev().collect()
    }
}

// Alternative Solution: More basic iterator solution.
// Complexity: runtime O(n+m), space O(n+m) [due to intermediate rev_result, but you can ignore
// this and say O(1) if you want; it's more of a rust thing].

impl Solution {
    pub fn add_binary(a: String, b: String) -> String {
        let mut rev_result = Vec::new();

        let (mut a_iter, mut b_iter) = (a.chars().rev(), b.chars().rev());
        let mut carry_set = false;

        loop {
            let (a_next, b_next) = (a_iter.next(), b_iter.next());

            if a_next.is_none() && b_next.is_none() && !carry_set {
                break;
            }

            let (a_set, b_set) = (a_next.unwrap_or('0') == '1', b_next.unwrap_or('0') == '1');
            let result_set = a_set ^ b_set ^ carry_set;
            carry_set = (a_set & b_set) | ((a_set | b_set) & carry_set);

            rev_result.push(if result_set { '1' } else { '0' });
        }

        rev_result.iter().rev().collect()
    }
}
