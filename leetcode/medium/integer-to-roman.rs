// https://leetcode.com/problems/integer-to-roman
//
// Complexity: runtime O(n), space O(n).

impl Solution {
    pub fn int_to_roman(mut num: i32) -> String {
        std::iter::from_fn(|| {
            Solution::ROMANS
                .iter()
                .find(|&&(val, _)| num >= val)
                .map(|&(val, roman)| {
                    num -= val;
                    roman.chars()
                })
        })
        .flatten()
        .collect()
    }

    const ROMANS: [(i32, &'static str); 13] = [
        (1000, "M"),
        (900, "CM"),
        (500, "D"),
        (400, "CD"),
        (100, "C"),
        (90, "XC"),
        (50, "L"),
        (40, "XL"),
        (10, "X"),
        (9, "IX"),
        (5, "V"),
        (4, "IV"),
        (1, "I"),
    ];
}
