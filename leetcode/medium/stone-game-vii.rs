// https://leetcode.com/problems/stone-game-vii
//
// Dynamic Programming (Memoization).
// Complexity: runtime O(n^2), space O(n^2).

impl Solution {
    pub fn stone_game_vii(stones: Vec<i32>) -> i32 {
        let mut mem = vec![vec![None; stones.len() + 1]; stones.len() + 1];
        mem.iter_mut().for_each(|r| r[0] = Some(0));
        Self::solve(&stones[..], stones.iter().sum(), 0, &mut mem)
    }

    fn solve(stones: &[i32], sum: i32, i: usize, mem: &mut Vec<Vec<Option<i32>>>) -> i32 {
        let n = stones.len();
        if mem[i][n].is_none() {
            let (lsum, rsum) = (sum - stones[0], sum - stones[n - 1]);
            let l = lsum - Self::solve(&stones[1..], lsum, i + 1, mem);
            let r = rsum - Self::solve(&stones[..n - 1], rsum, i, mem);
            mem[i][n] = Some(l.max(r));
        }
        mem[i][n].unwrap()
    }
}
