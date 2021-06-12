// https://leetcode.com/problems/minimum-number-of-refueling-stops
//
// Iterative DP (Memoization).
//
// I'm surprised this was a hard problem; I usually suck at coming up with iterative DP solutions
// without first doing the recursive one, but I was able to do so for this problem!
//
// Complexity: runtime O(n^2), space O(n).

impl Solution {
    pub fn min_refuel_stops(target: i32, start_fuel: i32, stations: Vec<Vec<i32>>) -> i32 {
        let mut mem = vec![start_fuel; stations.len() + 1];
        for (i, s) in stations.iter().enumerate() {
            for j in (0..=i).rev() {
                if s[0] <= mem[j] {
                    mem[j + 1] = mem[j + 1].max(s[1] + mem[j]);
                }
            }
        }
        mem.iter().position(|&d| d >= target).map_or(-1, |i| i as _)
    }
}
