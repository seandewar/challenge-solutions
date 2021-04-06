// https://leetcode.com/problems/minimum-operations-to-make-array-equal
//
// This is more of a maths problem.
//
// We have to consider both an odd-length and even-length array:
// [1, 3, 5] and [1, 3, 5, 7], for example.
// First, we get the median; this is 3 and 4 for both arrays respectively.
//
// > We can find this value via: (n / 2) * 2 + n % 2.
//
// Second, we determine the min number of ops required to turn the left-most and right-most values
// in the arrays to this median. We use the left/right-most values, as these values take the most
// ops out of any values in the array to get to the medium in the least number of ops, as they have
// the largest difference to the median.
//
// > This value is simply: median - 1, as it's the difference between the left-most value (1) and
//   the median (this will also apply to the right-most value, whatever it is).
//
// Finally, we need to adjust for the other values between the left/right-most value and the
// median; as these values increase/decrease by 2 each time, they become 2 closer to the median at
// each step.
//
// > We can adjust the summation formula for N natural numbers for this: 1/2 * N * (N + 1).
//   The number of elements between the median and the left/right-most is: (n / 2) - 1.
//   Plug this into the formula to get: 1/2 * (n / 2 - 1) * (n / 2 - 1 + 1)
//
// > Working from and simplifying from the above, we get (n / 2) * (n / 2 + n % 2) as our answer,
//   which can again be simplified to n * n / 4.
//
// Complexity: runtime O(1), space O(1).

impl Solution {
    pub fn min_operations(n: i32) -> i32 {
        n * n / 4 // equivalent to (n / 2) * (n / 2 + n % 2)
    }
}
