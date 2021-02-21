// https://leetcode.com/problems/broken-calculator
//
// The idea to solving this problem without recursion and dynamic programming is to work backwards.
// Even with memoization, the naive approach is too slow to pass.
//
// Rather than try to get x -> y, we instead get y -> x by attempting reverse operations (division
// by 2 and incrementing, rather than multiplication by 2 and decrementing). We can only divide if
// y is even, so we increment y in this case instead. Similarly, if y < x, the only reverse
// operation we have available is to increment y (x - y) times until y == x.
//
// The solution becomes trivial this way; admittedly, I missed this approach!
//
// Complexity: runtime O(logy), space O(1).

impl Solution {
    pub fn broken_calc(x: i32, mut y: i32) -> i32 {
        let mut num_iterations = 0;
        while y > x {
            y = if y % 2 == 0 { y / 2 } else { y + 1 };
            num_iterations += 1;
        }

        num_iterations + x - y // if x > y, we only need to increment y (decrement x)
    }
}
