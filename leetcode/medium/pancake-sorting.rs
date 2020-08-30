// https://leetcode.com/problems/pancake-sorting/
//
// Uses at most (a.len() - 1) * 2 flips (O(a.len()) flips).
//
// NOTE: At the time of writing, on 30th August 2020, the leetcode question for this problem
// incorrectly states that output values should be in the range [0, a.len), when in actuality, the
// tests expect [1, a.len]; this is why + 1 is added to the values pushed to the 'result' Vec.
//
// Complexity: runtime O(n), space O(1).

impl Solution {
    pub fn pancake_sort(mut a: Vec<i32>) -> Vec<i32> {
        let mut result = Vec::with_capacity((a.len() - 1) * 2);

        // no need to sort anything when the unsorted range has only 1 value (that value is
        // technically already sorted), hence we start the range at 1.. rather than 0..
        for unsorted_end_idx in (1..a.len()).rev() {
            let unsorted = &mut a[0..=unsorted_end_idx];
            let (max_idx, _) = unsorted.iter().enumerate().max_by_key(|&(_, x)| x).unwrap();

            // no need to do any flips if the max value in the unsorted range is already at the end
            // (it's already part of the sorted range)
            if (max_idx == unsorted_end_idx) {
                continue;
            }

            // no need to flip the max value in the unsorted range to the beginning if it's already
            // at the beginning of the range
            if (max_idx > 0) {
                unsorted[0..=max_idx].reverse();
                result.push((max_idx + 1) as i32);
            }

            unsorted.reverse();
            result.push((unsorted_end_idx + 1) as i32);
        }

        result
    }
}
