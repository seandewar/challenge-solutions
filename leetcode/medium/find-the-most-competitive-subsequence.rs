// https://leetcode.com/problems/find-the-most-competitive-subsequence
//
// Greedy stack-based solution.
//
// Create a stack with max size k and iterate over nums:
//
// If the current element in nums is larger than that at the top of our stack, or if our stack is
// empty, push the element onto our stack (unless it is full); if not, pop the stack until the
// condition is met. Continue iterating to the next element in nums.
//
// If the number of elements left to iterate over in nums is equal to the unused size of the stack,
// just fill the stack with what's remaining in nums.
//
// Complexity: runtime O(n), space O(n).

impl Solution {
    pub fn most_competitive(nums: Vec<i32>, k: i32) -> Vec<i32> {
        let k = k as usize;
        let mut res = Vec::with_capacity(k);
        for (i, &x) in nums.iter().enumerate() {
            while res.last().map_or(false, |&y| y > x) && nums.len() - i > k - res.len() {
                res.pop();
            }
            if res.len() < k {
                res.push(x);
            }
        }

        res
    }
}
