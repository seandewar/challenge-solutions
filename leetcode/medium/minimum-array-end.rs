// https://leetcode.com/problems/minimum-array-end
// Faster than 100%.
// Complexity: runtime O(logn), space O(1).

impl Solution {
    pub fn min_end(mut n: i32, x: i32) -> i64 {
        let (mut n, x) = (u64::try_from(n).unwrap() - 1, u64::try_from(x).unwrap());
        let mut ret = x;
        let mut mask: u64 = 1;
        while n > 0 {
            if x & mask == 0 {
                ret |= mask * (n & 1);
                n >>= 1;
            }
            mask <<= 1;
        }
        ret.try_into().unwrap()
    }
}
