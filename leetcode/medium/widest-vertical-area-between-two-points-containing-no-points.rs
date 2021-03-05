// https://leetcode.com/problems/widest-vertical-area-between-two-points-containing-no-points
//
// Sorting approach. Can also be done in linear-time if you'd prefer.
//
// Complexity: runtime O(n*logn), space O(logn).

impl Solution {
    pub fn max_width_of_vertical_area(mut points: Vec<Vec<i32>>) -> i32 {
        points.sort_unstable_by_key(|p| p[0]);
        (1..points.len())
            .map(|i| (points[i][0] - points[i - 1][0]).abs())
            .max()
            .unwrap()
    }
}
