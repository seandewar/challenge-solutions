// https://leetcode.com/problems/coin-change
//
// Dynamic Programming: Bottom-Top.
// Complexity: runtime O(n*m), space O(n).

impl Solution {
    pub fn coin_change(coins: Vec<i32>, amount: i32) -> i32 {
        let mut mem = vec![None; amount as usize + 1];
        mem[0] = Some(0);

        for i in 0..mem.len() - 1 {
            if let Some(steps) = mem[i] {
                for &c in &coins {
                    if let Some(next) = mem.get_mut(i + c as usize) {
                        next.replace(next.map_or(steps + 1, |n| n.min(steps + 1)));
                    }
                }
            }
        }

        mem[amount as usize].unwrap_or(-1)
    }
}
