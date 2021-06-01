// https://leetcode.com/problems/queries-on-number-of-points-inside-a-circle
//
// Complexity: runtime O(q*p), space O(1) [where q == queries.len(), p == points.len()]

impl Solution {
    pub fn count_points(points: Vec<Vec<i32>>, queries: Vec<Vec<i32>>) -> Vec<i32> {
        queries
            .iter()
            .map(|q| {
                points
                    .iter()
                    .filter(|p| q[2].pow(2) >= (p[0] - q[0]).pow(2) + (p[1] - q[1]).pow(2))
                    .count() as _
            })
            .collect()
    }
}
