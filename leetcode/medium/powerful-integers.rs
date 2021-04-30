// https://leetcode.com/problems/powerful-integers
//
// Complexity: runtime O(logx(bound) * logy(bound)), space O(logx(bound) * logy(bound)).

impl Solution {
    pub fn powerful_integers(x: i32, y: i32, bound: i32) -> Vec<i32> {
        let mut res = std::collections::HashSet::new();

        let (mut a, mut prev_a) = (1, 0);
        while a + 1 <= bound && a + 1 != prev_a + 1 {
            let (mut b, mut prev_b) = (1, 0);
            while a + b <= bound && a + b != a + prev_b {
                res.insert(a + b);
                prev_b = b;
                b *= y;
            }

            prev_a = a;
            a *= x;
        }

        res.into_iter().collect()
    }
}
