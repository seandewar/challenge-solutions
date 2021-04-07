// https://leetcode.com/problems/combination-sum-iii
//
// DFS + Back-tracking.
// Complexity: runtime O(), space O(1) [ignoring output; max DFS depth & aux.len() is 9].

impl Solution {
    pub fn combination_sum3(k: i32, n: i32) -> Vec<Vec<i32>> {
        let (mut res, mut aux) = (vec![], Vec::with_capacity(k as _));
        Self::dfs(n, &mut res, &mut aux);
        res
    }

    fn dfs(n: i32, out: &mut Vec<Vec<i32>>, aux: &mut Vec<i32>) {
        if aux.len() == aux.capacity() {
            if n == 0 {
                out.push(aux.clone());
            }
        } else if n > 0 {
            for i in *aux.last().unwrap_or(&0) + 1..=9 {
                aux.push(i);
                Self::dfs(n - i, out, aux);
                aux.pop();
            }
        }
    }
}
