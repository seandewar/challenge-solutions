// https://leetcode.com/problems/find-numbers-with-even-number-of-digits
//
// Complexity: runtime O(n*logk), space O(1).

impl Solution {
    pub fn find_numbers(nums: Vec<i32>) -> i32 {
        nums.iter()
            .filter(|&&x| {
                let (mut x, mut digits) = (x, 0);
                while x > 0 {
                    x /= 10;
                    digits += 1;
                }
                digits % 2 == 0
            })
            .count() as _
    }
}

// Alternative Solution: Using base 10 log (log10()).
// Complexity: runtime O(n*logk), space O(1).

impl Solution {
    pub fn find_numbers(nums: Vec<i32>) -> i32 {
        nums.iter()
            .filter(|&&x| (x as f64).log10() as _ % 2 == 0)
            .count() as _
    }
}
