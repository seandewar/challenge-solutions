// https://leetcode.com/problems/find-center-of-star-graph
//
// Complexity: runtime O(1), space O(1).

impl Solution {
    pub fn find_center(edges: Vec<Vec<i32>>) -> i32 {
        if edges[0][0] == edges[1][0] || edges[0][0] == edges[1][1] {
            edges[0][0]
        } else {
            edges[0][1]
        }
    }
}
