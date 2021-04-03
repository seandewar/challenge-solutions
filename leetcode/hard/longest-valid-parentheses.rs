// https://leetcode.com/problems/longest-valid-parentheses
//
// This solution is non-obvious, but:
//
// By getting the start index of the outer brace upon finding a ')', we can calculate the length of
// the valid parentheses sub-string enclosed by the outer braces that ends at this ')'.
//
// The best way to understand is to run this algorithm in your head. :)
//
// We have to initially push -1 to our stack so that the outer-most parentheses can be thought of
// as being surrounded by an invisible set of braces that cover the entire length of s.
//
// Complexity: runtime O(n), space O(n).

impl Solution {
    pub fn longest_valid_parentheses(s: String) -> i32 {
        let (mut stack, mut max) = (vec![-1_isize], 0);
        for (i, c) in s.chars().enumerate() {
            let i = i as isize;
            match c {
                '(' => stack.push(i),
                ')' => {
                    stack.pop();
                    if let Some(prev_i) = stack.last() {
                        max = max.max(i - prev_i);
                    } else {
                        stack.push(i);
                    }
                }
                _ => unreachable!(),
            }
        }

        max as _
    }
}

// Alternative Solution: Stack + BTreeMap.
// Complexity: runtime O(n*logn), space O(n).

use std::collections::BTreeMap;

impl Solution {
    pub fn longest_valid_parentheses(s: String) -> i32 {
        let (mut valids, mut stack) = (BTreeMap::new(), vec![]);
        for (i, c) in s.chars().enumerate() {
            match c {
                '(' => stack.push(i),
                ')' => {
                    if let Some(start) = stack.pop() {
                        // merge inner valid parentheses strings into this one
                        valids
                            .range(start + 1..i)
                            .map(|(&i, _)| i)
                            .collect::<Vec<_>>()
                            .into_iter()
                            .for_each(|i| {
                                valids.remove(&i);
                            });
                        valids.insert(start, i);
                    }
                }
                _ => unreachable!(),
            }
        }

        // longest valid string is the total length of the set of valid strings of outer-most
        // parentheses that are next to each other. E.g:
        //
        // ()()(()(()))(()())
        // [][][______][____]
        // [________________] has total size 2 + 2 + 8 + 6 == 18
        valids
            .into_iter()
            .scan((0, 0), |state, (i, j)| {
                let (len, expect_i) = *state;
                *state = (if expect_i == i { len } else { 0 } + j - i + 1, j + 1);
                Some(state.0)
            })
            .max()
            .unwrap_or(0) as _
    }
}
