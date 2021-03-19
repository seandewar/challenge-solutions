// https://leetcode.com/problems/k-th-symbol-in-grammar
//
// Recursive tree-like traversal.
// This can be optimized further and be made tail-recursive!
//
// Complexity: runtime O(n), space O(n).

impl Solution {
    pub fn kth_grammar(n: i32, k: i32) -> i32 {
        fn dfs(n: i32, k: i32, i: i32, val: i32) -> Option<i32> {
            if n == 1 {
                // (i == k).then_some(val) is preferred here, but Leetcode uses Rust 1.40
                Some(val).filter(|_| i == k)
            } else if i << (n - 1) < k {
                None
            } else {
                dfs(n - 1, k, i * 2 - 1, val).or_else(|| dfs(n - 1, k, i * 2, !val & 1))
            }
        }

        dfs(n, k, 1, 0).expect("row N has no index K")
    }
}

// Alternative Solution: Popcount.
// Complexity: runtime O(1), space O(1).

impl Solution {
    pub fn kth_grammar(_n: i32, k: i32) -> i32 {
        ((k - 1).count_ones() & 1) as _
    }
}

// Alternative Solution: Meme version of the first.
// Complexity: runtime O(n), space O(n).

impl Solution {
    pub fn kth_grammar(n: i32, k: i32) -> i32 {
        fn dfs(n: i32, k: i32, i: i32, val: i32) -> Option<i32> {
            Some(())
                .filter(|_| n == 1)
                .map(|_| Some(val).filter(|_| i == k))
                .or_else(|| {
                    Some(()).filter(|_| i << (n - 1) >= k).map(|_| {
                        dfs(n - 1, k, i * 2 - 1, val).or_else(|| dfs(n - 1, k, i * 2, !val & 1))
                    })
                })
                .flatten()
        }

        dfs(n, k, 1, 0).expect("row N has no index K")
    }
}
