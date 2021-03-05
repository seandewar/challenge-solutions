// https://leetcode.com/problems/group-the-people-given-the-group-size-they-belong-to
//
// Complexity: runtime O(n), space O(n).

impl Solution {
    pub fn group_the_people(group_sizes: Vec<i32>) -> Vec<Vec<i32>> {
        let mut group_map = std::collections::HashMap::new();
        group_sizes
            .into_iter()
            .enumerate()
            .for_each(|(i, size)| group_map.entry(size as _).or_insert(vec![]).push(i as _));

        group_map
            .iter()
            .flat_map(|(&size, people)| people.chunks(size).map(|group| group.into()))
            .collect()
    }
}
