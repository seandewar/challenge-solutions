// https://leetcode.com/problems/flower-planting-with-no-adjacent
//
// Greedy solution.
//
// Because there are 4 possible colours for 3 possible paths from/into a node, we can simply choose
// the colour for each node that hasn't been assigned to any adjacent nodes; there always exists 1
// such colour.
//
// Complexity: runtime O(v+e), space O(e) [O(v+e) including output].

impl Solution {
    pub fn garden_no_adj(n: i32, paths: Vec<Vec<i32>>) -> Vec<i32> {
        let mut graph = std::collections::HashMap::new(); // adjacency map
        for p in paths {
            graph
                .entry(p[0])
                .or_insert(Vec::with_capacity(3))
                .push(p[1] as usize);
            graph
                .entry(p[1])
                .or_insert(Vec::with_capacity(3))
                .push(p[0] as usize);
        }

        let mut res = vec![0; n as _];
        for i in 1..=n {
            let mut used_colors = [false; 4];
            for &d in graph
                .get(&i)
                .iter()
                .flat_map(|d_opt| d_opt.iter())
                .filter(|&d| res[d - 1] != 0)
            {
                used_colors[res[d - 1] as usize - 1] = true;
            }

            res[i as usize - 1] = used_colors
                .iter()
                .enumerate()
                .filter_map(|(i, &is_unused)| Some((i + 1) as _).filter(|_| !is_unused))
                .next()
                .expect("4 possible colours for 3 paths implies that >= 1 colour is unused");
        }

        res
    }
}
