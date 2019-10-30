// https://leetcode.com/problems/fibonacci-number/
//
// Complexity: runtime O(n), space O(1).

impl Solution {
    pub fn fib(n: i32) -> i32 {
        (0..n)
            .fold((0, 1), |(fib, next_fib), _| (next_fib, fib + next_fib))
            .0
    }
}
