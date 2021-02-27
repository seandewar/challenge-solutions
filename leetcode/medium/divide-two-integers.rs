// https://leetcode.com/problems/divide-two-integers
//
// Complexity: runtime O(~log32) == O(1), space O(1).

impl Solution {
    pub fn divide(mut dividend: i32, mut divisor: i32) -> i32 {
        assert!(divisor != 0);

        let is_ans_neg = (dividend < 0) ^ (divisor < 0);
        dividend = -dividend.abs();
        divisor = -divisor.abs();

        let (mut neg_ans, mut curr_div, mut curr_mul) = (0, divisor, -1);
        while curr_div <= divisor {
            if curr_div < dividend {
                curr_div = if curr_div == -1 { 0 } else { curr_div >> 1 };
                curr_mul = if curr_mul == -1 { 0 } else { curr_mul >> 1 };
            } else {
                neg_ans += curr_mul;
                dividend -= curr_div;
                curr_div <<= 1;
                curr_mul <<= 1;
            }
        }

        match neg_ans {
            std::i32::MIN if !is_ans_neg => std::i32::MAX,
            _ if is_ans_neg => neg_ans,
            _ => -neg_ans,
        }
    }
}
