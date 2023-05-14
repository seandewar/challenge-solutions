use std::{marker::PhantomData, ptr::NonNull};

mod pre_implemented;

pub struct LinkedList<T> {
    front: NodePtr<T>,
    // Also store the `len` and a pointer to the `back` node to make such queries O(1).
    // As we know the length, we can use `self.len == 0` as a sentinel for an empty list,
    // therefore there is no need to wrap `front` or `back` inside an `Option`.
    back: NodePtr<T>,
    len: usize,
}

type NodePtr<T> = NonNull<Node<T>>;

struct Node<T> {
    next: Option<NodePtr<T>>,
    prev: Option<NodePtr<T>>,
    data: T,
}

impl<T> LinkedList<T> {
    pub fn new() -> Self {
        Self {
            front: NonNull::dangling(),
            back: NonNull::dangling(),
            len: 0,
        }
    }

    pub fn len(&self) -> usize {
        self.len
    }

    pub fn is_empty(&self) -> bool {
        self.len == 0
    }

    pub fn cursor_front(&mut self) -> Cursor<'_, T> {
        Cursor {
            curr: (!self.is_empty()).then_some(self.front),
            list: self,
        }
    }

    pub fn cursor_back(&mut self) -> Cursor<'_, T> {
        Cursor {
            curr: (!self.is_empty()).then_some(self.back),
            list: self,
        }
    }

    pub fn iter(&self) -> Iter<'_, T> {
        Iter {
            next: (!self.is_empty()).then_some(self.front),
            list: PhantomData,
        }
    }
}

impl<T> Default for LinkedList<T> {
    fn default() -> Self {
        Self::new()
    }
}

impl<T> Drop for LinkedList<T> {
    fn drop(&mut self) {
        if self.is_empty() {
            return;
        }

        let mut node = self.front;
        while let Some(next) = unsafe { Box::from_raw(node.as_ptr()) }.next {
            node = next;
        }
    }
}

unsafe impl<T: Send> Send for LinkedList<T> {}
unsafe impl<T: Sync> Sync for LinkedList<T> {}

pub struct Cursor<'a, T> {
    curr: Option<NodePtr<T>>,
    list: &'a mut LinkedList<T>,
}

impl<'a, T> Cursor<'a, T> {
    pub fn peek_mut(&mut self) -> Option<&'a mut T> {
        self.curr.map(|mut p| &mut unsafe { p.as_mut() }.data)
    }

    #[allow(clippy::should_implement_trait)]
    pub fn next(&mut self) -> Option<&'a mut T> {
        let curr = self.curr.map(|mut p| unsafe { p.as_mut() })?;
        self.curr = Some(curr.next?);
        self.peek_mut()
    }

    pub fn prev(&mut self) -> Option<&'a mut T> {
        let curr = self.curr.map(|mut p| unsafe { p.as_mut() })?;
        self.curr = Some(curr.prev?);
        self.peek_mut()
    }

    pub fn take(&mut self) -> Option<T> {
        let boxed_curr = unsafe { Box::from_raw(self.curr?.as_ptr()) };

        if let Some(mut prev) = boxed_curr.prev {
            unsafe { prev.as_mut() }.next = boxed_curr.next;
            if boxed_curr.next.is_none() {
                self.list.back = prev;
            }
        }
        if let Some(mut next) = boxed_curr.next {
            unsafe { next.as_mut() }.prev = boxed_curr.prev;
            if boxed_curr.prev.is_none() {
                self.list.front = next;
            }
        }
        self.list.len -= 1;

        self.curr = if let Some(next) = boxed_curr.next {
            Some(next)
        } else {
            boxed_curr.prev
        };
        Some(boxed_curr.data)
    }

    pub fn insert_after(&mut self, data: T) {
        let node = unsafe {
            NonNull::new_unchecked(Box::into_raw(Box::new(Node {
                next: self.curr.and_then(|p| p.as_ref().next),
                prev: self.curr,
                data,
            })))
        };

        if let Some(curr) = self.curr.map(|mut p| unsafe { p.as_mut() }) {
            if let Some(next) = curr.next.map(|mut p| unsafe { p.as_mut() }) {
                next.prev = Some(node);
            } else {
                self.list.back = node;
            }
            curr.next = Some(node);
        } else {
            self.list.front = node;
            self.list.back = node;
        }
        self.list.len += 1;
    }

    pub fn insert_before(&mut self, data: T) {
        let node = unsafe {
            NonNull::new_unchecked(Box::into_raw(Box::new(Node {
                next: self.curr,
                prev: self.curr.and_then(|p| p.as_ref().prev),
                data,
            })))
        };

        if let Some(curr) = self.curr.map(|mut p| unsafe { p.as_mut() }) {
            if let Some(prev) = curr.prev.map(|mut p| unsafe { p.as_mut() }) {
                prev.next = Some(node);
            } else {
                self.list.front = node;
            }
            curr.prev = Some(node);
        } else {
            self.list.front = node;
            self.list.back = node;
        }
        self.list.len += 1;
    }
}

pub struct Iter<'a, T> {
    next: Option<NodePtr<T>>,
    list: PhantomData<&'a LinkedList<T>>,
}

impl<'a, T> Iterator for Iter<'a, T> {
    type Item = &'a T;

    fn next(&mut self) -> Option<Self::Item> {
        if let Some(next) = self.next.map(|p| unsafe { p.as_ref() }) {
            self.next = next.next;
            Some(&next.data)
        } else {
            None
        }
    }
}
