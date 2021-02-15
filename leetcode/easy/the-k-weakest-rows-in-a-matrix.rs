// https://leetcode.com/problems/the-k-weakest-rows-in-a-matrix
//
// Complexity: runtime O(m*n+n*logn), space O(n).

impl Solution {
    pub fn k_weakest_rows(mat: Vec<Vec<i32>>, k: i32) -> Vec<i32> {
        // collect into a Vec of (row sum, row index) tuples
        let mut row_sums_to_idxs = mat
            .into_iter()
            .enumerate()
            .map(|(i, r)| (r.into_iter().sum::<i32>(), i))
            .collect::<Vec<_>>();

        // sort; if many rows have the same sum, they will instead be sorted by index, as Rust uses
        // a lexicographical ordering based on the order of tuple elements (which is why we have
        // the row index as the 2nd element in our tuples)
        row_sums_to_idxs.sort_unstable();

        // collect into a Vec of the k row indices to return
        row_sums_to_idxs
            .into_iter()
            .map(|(_, i)| i as i32)
            .take(k as _)
            .collect()
    }
}

// Alternative Solution: Only sort the k indices we need to return.
//
// Rust 1.40 (the version that LeetCode uses) doesn't really have a nice equivalent to C++'s
// std::partial_sort() (at least Rust 1.49 has select_nth_unsorted(), which can achieve the same).
//
// Instead, we can use a BinaryHeap, where the heapification is O(n) and the popping into a sorted
// Vec of k elements is O(k*logn).
//
// Complexity: runtime O(m*n+k*logn), space O(n).

impl Solution {
    pub fn k_weakest_rows(mat: Vec<Vec<i32>>, k: i32) -> Vec<i32> {
        // as BinaryHeap is a max-heap by default, we use Reverse() to reverse the ordering of our
        // tuples, so that our heap becomes a min-heap (so we sort in ascending order)
        let mut row_sums_to_idxs = mat
            .into_iter()
            .enumerate()
            .map(|(i, r)| std::cmp::Reverse((r.into_iter().sum::<i32>(), i)))
            .collect::<Vec<_>>();

        // BinaryHeap::from() uses "heapification", which has time complexity O(n) (not the
        // O(n*logn) you'd typically expect if we push() each element into the heap manually)
        let mut heap = std::collections::BinaryHeap::from(row_sums_to_idxs);

        // BinaryHeap::into_iter_sorted() would be much nicer, but it hasn't been stabilized yet,
        // so we'll need to pop the k elements out manually (if we use into_sorted_vec(), we'll end
        // up popping out all n elements, which would defeat the purpose of our optimization)
        let mut result = Vec::with_capacity(k as _);
        for _ in 0..k {
            result.push((heap.pop().unwrap().0).1 as i32);
        }

        result
    }
}
