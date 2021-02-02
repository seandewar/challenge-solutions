// https://leetcode.com/problems/create-sorted-array-through-instructions
//
// Binary Indexed Tree (Fenwick Tree) Solution.
//
// To pass this problem, we'll need to calculate the cumulative frequency of the instructions in a
// sorted array of instructions, so that we can calculate the costs associated with each insertion.
//
// We could try to do this naively, by creating a new array in which we place our instructions in
// sorted order, calculating the cost as we go, however this is too slow; we need something faster!
//
// For problems such as these, where we need to create a cumulative frequency array from an array
// of frequencies, we generally have 2 approaches:
//
// 1. Populate an array of cumulative frequencies; querying the cumulative frequency from index 0
//    to some index i is as simple as accessing the element at index i. However, updating the value
//    of a particular frequency at i involves updating the cumulative frequencies from index i to
//    (n-1).
//
//    Time complexities: update frequency O(n), query sum O(1).
//
// 2. Populate an array of individual frequencies; querying the cumulative frequency from index 0
//    to some index i is more involved, requiring us to sum the frequencies from index 0 to i.
//    However, updating the value of a particular frequency only involves us updating the value at
//    i.
//
//    Time complexities: update frequency O(1), query sum O(n).
//
// There exists a more involved approach using Fenwick Trees, which instead has O(logn) complexity
// for both the update frequency and query sum operations; we'll be relying on both operations in
// this solution, making this approach very valuable.
//
// The idea is, rather than store the cumulative frequency for the range [0, i] at some index i, we
// store the cumulative frequency for some smaller range [a, i]. This allows us to calculate the
// frequency sum for a larger range [0, i] by only summing the stored frequency values for the
// smaller ranges, of which there is less than (i+1) to consider. Fenwick Trees achieve this by n
// cumulative frequencies for n different ranges of different logarithmic-based sizes using the
// binary representation of each index i (usually 1-indexed for simplicity of implementation).
// Fenwick Trees are usually implemented using plain-old arrays; no tree node pointers necessary!
//
// For example, consider the array: [1, 2, 4, 3, 1, 4].
//
// We can create a frequency array of size 4, with 1-based index values to store the frequency of
// elements 1, 2, 3 and 4: values at indices [1, 2, 3, 4] = [2, 1, 1, 2] respectively.
// A cumulative frequency table of the above would look like this: [2, 3, 4, 6]. Index values will
// be assumed 1-based from now on.
//
// Now, if we write the binary representations of each of the elements, you can see an example of
// how we would split the cumulative frequencies in a Fenwick Tree:
//
// 1 = 001 ]1 ]  ]    ranges covered: 1: [1, 1]
// 2 = 010    ]2 ]                    2: [1, 2]
// 3 = 011 ]3    ]                    3: [3, 3]
// 4 = 100       ]4                   4: [1, 4]
//
// We instead have 4 distinct cumulative frequencies for ranges of different sizes. For example:
// If we want the cumulative frequency [1, 4], we just read the value at index 4.
// If we want the cumulative frequency [1, 3], we sum the values at indices 2 (covering [1, 2]) and
// 3 (covering [3, 3]).
//
// You will see that the ranges covered by each of the individual cumulative frequencies depends on
// the position of the right-most set bit in the index. Explicitly, the size is 2^d, where d is the
// distance of the set bit's position from the right-most bit position.
//
// To easily isolate the right-most set bit from an index i, we can simply calculate (i & -i). If
// we add this bit to i, we get the index of the cumulative sum for the next-largest range that
// encompasses i; otherwise, if we subtract the bit from i, we get the index of the sum for the
// next-largest range ending at (i-1).
//
// Therefore, to update the frequency for an index i, we want to add the right-most set bit in i to
// i, so that we can update the cumulative frequencies for all ranges encompassing i; we do this
// until i is > than the highest-possible index in the Fenwick Tree. If we want to get the
// cumulative frequency for some range [1, i], we subtract the right-most set bit in i from i, so
// that we only add the ranges necessary to cover [1, i] with no overlapping (otherwise we'd end up
// counting some frequency values multiple times); we do this until i is <= 0.
//
// I feel like it really helps to watch some videos on this topic or at least look at some
// diagrams, as it can be quite hard to visualize and comprehend this at times. Luckily, the
// implementation is rather simple!
//
// You may also solve this problem using Segment Trees, but those are much more complicated, and
// usually less space-optimized.
//
// Complexity: runtime O(n*logn), space O(1).

impl Solution {
    pub fn create_sorted_array(instructions: Vec<i32>) -> i32 {
        fn bit_inc(bit: &mut [i32], mut x: i32) {
            while (x as usize) < bit.len() {
                bit[x as usize] += 1;
                x += x & -x;
            }
        }
        fn bit_sum(bit: &[i32], mut x: i32) -> i32 {
            let mut sum = 0;
            while x > 0 {
                sum += bit[x as usize];
                x -= x & -x;
            }

            sum
        }

        let mut bit = [0; 100_001];
        let mut res = 0u64;

        for (i, x) in instructions.into_iter().enumerate() {
            let num_before = bit_sum(&bit[..], x - 1);
            let num_after = i as i32 - bit_sum(&bit[..], x);
            res += num_before.min(num_after) as u64;
            bit_inc(&mut bit[..], x);
        }

        (res % 1_000_000_007) as _
    }
}
