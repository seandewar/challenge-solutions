// https://leetcode.com/problems/subtract-the-product-and-sum-of-digits-of-an-integer
//
// Complexity: runtime O(logn), space O(1).

impl Solution {
    pub fn subtract_product_and_sum(mut n: i32) -> i32 {
        assert!(n >= 0);

        let (mut product, mut sum) = (1, 0);
        while n != 0 {
            product *= n % 10;
            sum += n % 10;
            n /= 10;
        }

        product - sum
    }
}
