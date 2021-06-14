// https://leetcode.com/problems/maximum-units-on-a-truck
//
// Complexity: runtime O(n*logn), space O(logn).

impl Solution {
    pub fn maximum_units(mut box_types: Vec<Vec<i32>>, mut truck_size: i32) -> i32 {
        box_types.sort_unstable_by_key(|b| std::cmp::Reverse(b[1]));
        box_types
            .iter()
            .map(|b| (b[0], b[1]))
            .fold(0, |acc, (n, units)| {
                let take = n.min(truck_size);
                truck_size -= take;
                acc + take * units
            })
    }
}
