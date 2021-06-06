// https://leetcode.com/problems/queries-on-a-permutation-with-key
//
// Complexity: runtime O(queries*m), space O(m).

impl Solution {
    pub fn process_queries(queries: Vec<i32>, m: i32) -> Vec<i32> {
        let mut p: std::collections::VecDeque<_> = (1..=m).collect();
        queries
            .iter()
            .map(|q| {
                let i = p.iter().position(|x| x == q).unwrap();
                let x = p.remove(i).unwrap();
                p.push_front(x);
                i as _
            })
            .collect()
    }
}
