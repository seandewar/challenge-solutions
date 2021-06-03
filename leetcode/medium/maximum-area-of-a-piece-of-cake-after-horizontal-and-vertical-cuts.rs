// https://leetcode.com/problems/maximum-area-of-a-piece-of-cake-after-horizontal-and-vertical-cuts
//
// Complexity: runtime O(h*logh + v*logv), space O(logh + logv).

impl Solution {
    pub fn max_area(h: i32, w: i32, mut h_cuts: Vec<i32>, mut v_cuts: Vec<i32>) -> i32 {
        let max = |cuts: &mut Vec<i32>, dim| {
            cuts.extend(&[0, dim]);
            cuts.sort_unstable();
            cuts.windows(2).map(|w| w[1] - w[0]).max().unwrap() as u64
        };
        (max(&mut h_cuts, h) * max(&mut v_cuts, w) % 1_000_000_007) as _
    }
}
