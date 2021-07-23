// https://leetcode.com/problems/find-k-closest-elements
//
// Complexity: runtime O(logn + k), space O(1).

impl Solution {
    pub fn find_closest_elements(arr: Vec<i32>, k: i32, x: i32) -> Vec<i32> {
        let dist_to_x = |&y: &i32| (x - y).abs();
        let mut i = arr.binary_search(&x).unwrap_or_else(|i| {
            let dleft = arr.get(i - 1).map(dist_to_x);
            let di = arr.get(i).map(dist_to_x);
            if dleft.is_none() || di.is_some() && di.unwrap() < dleft.unwrap() {
                i
            } else {
                i - 1
            }
        });
        let mut j = i;
        while j - i + 1 < k as usize {
            let di = arr.get(i - 1).map(dist_to_x);
            let dj = arr.get(j + 1).map(dist_to_x);
            if di == dj || di.is_some() && dj.map_or(true, |dj| di.unwrap() < dj) {
                i -= 1;
            } else {
                j += 1;
            }
        }
        arr[i..=j].into()
    }
}
