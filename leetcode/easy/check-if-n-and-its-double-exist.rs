// https://leetcode.com/problems/check-if-n-and-its-double-exist
//
// Complexity: runtime O(n), space O(n).

impl Solution {
    pub fn check_if_exist(arr: Vec<i32>) -> bool {
        let mut set = std::collections::HashSet::new();
        arr.iter().any(|&x| {
            let found = set.contains(&(x * 2)) || x % 2 == 0 && set.contains(&(x / 2));
            set.insert(x);
            found
        })
    }
}
