// https://leetcode.com/problems/validate-stack-sequences
//
// Complexity: runtime O(n), space O(n).

impl Solution {
    pub fn validate_stack_sequences(pushed: Vec<i32>, popped: Vec<i32>) -> bool {
        let mut stack = Vec::new();
        let mut pushed_iter = pushed.into_iter();
        let mut popped_iter = popped.into_iter().peekable();

        while let Some(next_to_pop) = popped_iter.peek() {
            if stack.last().map_or(false, |top| top == next_to_pop) {
                stack.pop();
                popped_iter.next();
            } else if let Some(next_to_push) = pushed_iter.next() {
                stack.push(next_to_push);
            } else {
                return false;
            }
        }

        true
    }
}

// Alternative Solution: No extra space.
// We can simply use "pushed" as both our input and as a "virtual" stack.
//
// Complexity: runtime O(n), space O(1).

impl Solution {
    pub fn validate_stack_sequences(mut pushed: Vec<i32>, popped: Vec<i32>) -> bool {
        let (mut stack_next_idx, mut next_to_push_idx) = (0, 0);
        let mut popped_iter = popped.into_iter().peekable();

        while let Some(to_pop) = popped_iter.peek() {
            if pushed
                .get(stack_next_idx - 1)
                .map_or(false, |top| top == to_pop)
            {
                stack_next_idx -= 1;
                popped_iter.next();
            } else if let Some(&to_push) = pushed.get(next_to_push_idx) {
                pushed[stack_next_idx] = to_push;
                stack_next_idx += 1;
                next_to_push_idx += 1;
            } else {
                return false;
            }
        }

        true
    }
}
