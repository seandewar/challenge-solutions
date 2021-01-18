// https://leetcode.com/problems/count-sorted-vowel-strings/
//
// Dynamic Programming Solution.
//
// Interestingly, I wasn't deliberately searching for a DP solution when coming up with this; the
// steps naturally fell into place after determining that this problem can likely be solved mostly
// mathematically. (It can indeed be solved completely mathematically in O(1) too! I'm too tired
// right now to describe that solution, though :[)
//
// The steps to take became clearer after I wrote calculated the solution for n=1 & 2:
//
// n=1: [(a), (e), (i), (o), (u)] = 5
// n=2: [(aa, ae, ai, ao, au), (ee, ei, eo, eu), (ii, io, iu), (oo, ou), (uu)] = 15
// ...
//
// If you group strings beginning with the same vowel together, and count the number, you can start
// to see a pattern:
//
// n=1: [(1), (1), (1), (1), (1)]   = 5
// n=2: [(5), (4), (3), (2), (1)]   = 15
// n=3: [(15), (10), (6), (3), (1)] = 35
// ...
//
// As you can see, for any value of n > 1, the length of the first group is the total number of
// vowel strings for the previous value of n. The length of each group after the first is always
// that of the previous, but minus the length of the group for the previous n that has the same
// index as the previous group for the current n, for all n > 1:
//
// n=1: [(1), (1), (1), (1), (1)]                     = 5 (the base case)
// n=2: [(5), (5-1=4), (4-1=3), (3-1=2), (2-1=1)]     = 15
// n=3: [(15), (15-5=10), (10-4=6), (6-3=3), (3-2=1)] = 35
// ...
//
// The following algorithm is an implementation of this rule. The groups are calculated in reverse
// order to simplify the implementation a little bit.
//
// Complexity: runtime O(n), space O(1).

impl Solution {
    pub fn count_vowel_strings(n: i32) -> i32 {
        let mut vowel_perms = [1; 5];
        for _ in 1..n {
            for i in 1..5 {
                vowel_perms[i] += vowel_perms[i - 1];
            }
        }

        vowel_perms.iter().sum()
    }
}
