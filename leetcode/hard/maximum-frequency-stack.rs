// https://leetcode.com/problems/maximum-frequency-stack
//
// Complexity: runtime new() O(1), push() O(logn), pop() O(logn), space O(n).

use std::collections::{BinaryHeap, HashMap};

#[derive(Default)]
struct FreqStack {
    max_heap: BinaryHeap<(usize, usize, i32)>,
    freqs: HashMap<i32, usize>,
    push_count: usize,
}

impl FreqStack {
    fn new() -> Self {
        Default::default()
    }

    fn push(&mut self, x: i32) {
        let freq = self.freqs.entry(x).or_default();
        *freq += 1;
        self.max_heap.push((*freq, self.push_count, x));
        self.push_count += 1;
    }

    fn pop(&mut self) -> i32 {
        let x = self.max_heap.pop().unwrap().2;
        *self.freqs.get_mut(&x).unwrap() -= 1;
        x
    }
}

// Alternative Solution: With constant-time operations.
// Complexity: runtime O(1) all operations, space O(n).

use std::collections::HashMap;

#[derive(Default)]
struct FreqStack {
    freqs: HashMap<i32, usize>,
    push_orders: HashMap<usize, Vec<i32>>,
    max_freq: usize,
}

impl FreqStack {
    fn new() -> Self {
        Default::default()
    }

    fn push(&mut self, x: i32) {
        let freq = self.freqs.entry(x).or_default();
        *freq += 1;
        self.push_orders.entry(*freq).or_default().push(x);
        self.max_freq = self.max_freq.max(*freq);
    }

    fn pop(&mut self) -> i32 {
        let orders = self.push_orders.get_mut(&self.max_freq).unwrap();
        let x = orders.pop().unwrap();
        if orders.is_empty() {
            self.max_freq -= 1;
        }

        *self.freqs.get_mut(&x).unwrap() -= 1;
        x
    }
}
