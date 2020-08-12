// https://leetcode.com/problems/pascals-triangle-ii/
//
// This can be done in O(k) runtime using binomial coefficients.
// Complexity: runtime O(k^2), space O(k).

impl Solution {
    pub fn get_row(row_index: i32) -> Vec<i32> {
        let mut result = vec![1; (row_index + 1) as usize];

        for i in 0..=row_index as usize {
            let mut prev = 1;

            for j in 1..i {
                let new_val = result[j] + prev;
                prev = std::mem::replace(&mut result[j], new_val);
            }
        }

        result
    }
}

// Alternative Solution: Manual + O(2*k) space.
// Complexity: runtime O(k^2), space O(2*k) = O(k).

impl Solution {
    pub fn get_row(row_index: i32) -> Vec<i32> {
        let mut result = vec![1];

        for i in 1..=row_index as usize {
            let mut row = vec![1; i + 1];

            for j in 1..i {
                row[j] = result[j - 1] + result[j];
            }

            result = row;
        }

        result
    }
}
