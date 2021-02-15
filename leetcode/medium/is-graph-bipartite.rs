// https://leetcode.com/problems/is-graph-bipartite
//
// I didn't actually know about the "two colour"-based approach and terminology; I came up with
// this solution myself that has the same idea (originally, the "colour" variable was named
// "order").
//
// We basically traverse the graph starting from each node we haven't visited yet, marking nodes
// with 1 of 2 "colours". Every time we step to a new node, we mark that new node with the
// *alternate* colour of the previous (node colours are represented via the 2 sets in
// "visited_sets"). If we visit a node that already has a colour, we need to check that the colour
// is the alternate of the one we previously visited; if we now get a different colour, the graph
// is *not* bipartite, and we return false; otherwise it is bipartite, and we return true.
//
// See code comments for some more details.
//
// Complexity: runtime O(v+e), space O(v).

use std::collections::HashSet;

impl Solution {
    pub fn is_bipartite(graph: Vec<Vec<i32>>) -> bool {
        fn dfs(
            graph: &Vec<Vec<i32>>,
            visited_sets: &mut [HashSet<i32>; 2],
            node: i32,
            colour: bool,
        ) -> bool {
            let (seta_idx, setb_idx) = if colour { (0, 1) } else { (1, 0) };
            if !visited_sets[seta_idx].insert(node) {
                return true; // visited already with the expected colour
            }
            if visited_sets[setb_idx].contains(&node) {
                return false; // visited already, but with an unexpected colour; not bipartite!
            }
            // DFS adjacent nodes with the alternate colour
            graph[node as usize]
                .iter()
                .all(|&n| dfs(graph, visited_sets, n, !colour))
        }

        // create our 2 sets (1 for each colour) and DFS from all unvisited nodes in the graph
        let mut visited_sets: [HashSet<i32>; 2] = Default::default();
        (0..graph.len() as i32).all(|n| {
            visited_sets.iter().any(|s| s.contains(&n)) || dfs(&graph, &mut visited_sets, n, true)
        })
    }
}
