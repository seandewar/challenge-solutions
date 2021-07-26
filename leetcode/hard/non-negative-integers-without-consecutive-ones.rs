// https://leetcode.com/problems/non-negative-integers-without-consecutive-ones
//
// This was not a fun problem, but once you see the Fibonacci pattern, it becomes easier, though
// the mechanism is kind of hard to explain.
//
// Complexity: runtime O(1), space O(1).

impl Solution {
    pub fn find_integers(n: i32) -> i32 {
        let n = n as u32;
        let fib: Vec<_> = (0..30)
            .scan((1, 2), |state, _| {
                let (a, b) = *state;
                *state = (b, a + b);
                Some(a)
            })
            .collect();

        (0..fib.len())
            .rev()
            .try_fold(1, |mut res, i| {
                if (n >> i) & 1 == 1 {
                    res += fib[i];
                    if (n >> i + 1) & 1 == 1 {
                        return Err(res - 1);
                    }
                }
                Ok(res)
            })
            .unwrap_or_else(|res| res) as _
    }
}
