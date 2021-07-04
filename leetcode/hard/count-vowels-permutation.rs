// https://leetcode.com/problems/count-vowels-permutation
//
// Recursive standard DP + Memoization approach.
// Complexity: runtime O(n), space O(n).

static VOWELS: &str = "aeiou";
const MOD: u64 = 1_000_000_007;

impl Solution {
    pub fn count_vowel_permutation(n: i32) -> i32 {
        let mut mem = vec![[0; 5]; n as _];
        (VOWELS
            .chars()
            .map(|c| Self::solve(&mut mem, c, (n - 1) as _))
            .sum::<u64>()
            % MOD) as _
    }

    fn solve(mem: &mut [[u64; 5]], c: char, n: usize) -> u64 {
        let i = VOWELS.chars().position(|x| c == x).unwrap();
        if mem[n][i] == 0 {
            mem[n][i] = if n == 0 {
                1
            } else {
                match c {
                    'a' => "e",
                    'e' => "ai",
                    'i' => "aeou",
                    'o' => "iu",
                    'u' => "a",
                    _ => unreachable!(),
                }
                .chars()
                .map(|x| Self::solve(mem, x, n - 1) % MOD)
                .sum()
            };
        }
        mem[n][i]
    }
}
