// https://leetcode.com/problems/design-circular-queue
//
// Complexity: runtime O(1) all operations (except O(n) construction), space O(n).

#[derive(Default)]
struct MyCircularQueue {
    buf: Vec<i32>,
    front: usize,
    len: usize,
}

impl MyCircularQueue {
    fn new(k: i32) -> Self {
        Self {
            buf: vec![0; k as _],
            ..Default::default()
        }
    }

    fn en_queue(&mut self, value: i32) -> bool {
        if self.is_full() {
            false
        } else {
            let cap = self.buf.len();
            self.len += 1;
            self.buf[(self.front + self.len - 1) % cap] = value;
            true
        }
    }

    fn de_queue(&mut self) -> bool {
        if self.is_empty() {
            false
        } else {
            self.len -= 1;
            self.front = (self.front + 1) % self.buf.len();
            true
        }
    }

    fn front(&self) -> i32 {
        if self.is_empty() {
            -1
        } else {
            self.buf[self.front]
        }
    }

    fn rear(&self) -> i32 {
        if self.is_empty() {
            -1
        } else {
            self.buf[(self.front + self.len - 1) % self.buf.len()]
        }
    }

    fn is_empty(&self) -> bool {
        self.len == 0
    }

    fn is_full(&self) -> bool {
        self.len == self.buf.len()
    }
}
