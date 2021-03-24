// https://leetcode.com/problems/advantage-shuffle
//
// Complexity: runtime O(n^2) worst-case due to remove() on each iteration, but runtime is likely
//             to be closer to O(n*logn),
//             space O(n) [O(logn) ignoring output (sort_unstable())].

impl Solution {
    pub fn advantage_count(mut a: Vec<i32>, b: Vec<i32>) -> Vec<i32> {
        a.sort_unstable();
        b.into_iter()
            .map(|x| {
                let i = a.binary_search(&(x + 1)).unwrap_or_else(|i| i);
                a.remove(if i < a.len() { i } else { 0 })
            })
            .collect()
    }
}

// Alternative Solution: In-place.
// Complexity: runtime O(n^2 * logn), space O(logn) [sort_unstable()].

impl Solution {
    pub fn advantage_count(mut a: Vec<i32>, b: Vec<i32>) -> Vec<i32> {
        a.sort_unstable();
        for i in 0..a.len() {
            let j = i + a[i..].binary_search(&(b[i] + 1)).unwrap_or_else(|j| j);
            if j < a.len() {
                a.swap(i, j);
                a[i + 1..].sort_unstable();
            }
        }

        a
    }
}
