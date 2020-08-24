// https://leetcode.com/problems/stream-of-characters/
//
// Trie + Query Vector solution.
//
// In other languages, this code would be quite a bit simpler. The issue here is that Rust's
// lifetime rules are a bit overzealous, and although our Vec of queries should only need to store
// references to TrieNodes on the heap (ideally, via Box), the lifetime rules wont allow us to do
// this, as semantically, we'll end up referencing the TrieNode objects contained within
// StreamChecker from references stored within StreamChecker (a self-reference). This isn't
// allowed, due to the potential of dangling references if a move occurs in cases where the
// referenced data stored in the struct is stored in the same frame as the struct (e.g. on the
// stack). Also, theoretically, we could delete or move some TrieNodes in our StreamChecker between
// calls to query(), which would invalidate references stored in the queries member. However, none
// of this actually happens in practice, and unfortunately, all this reference counting and
// interior-mutability checking does have a runtime cost.
//
// A nicer way of doing this in Rust is to either use unsafe code or the owning_ref crate (or
// possibly std::pin) to implement this. Alternatively, use HashMaps rather than Tries (storing
// each word and their prefixes as keys, at the expense of a larger memory footprint), or rather
// than store the position of each query in the trie, simply re-run each query from the beginning
// of the trie.
//
// Complexity: init runtime O(wc*wl), query() runtime O(wl), space O(wc*wl) [where wc = num words,
// wl = word length (upper bound)].

use std::cell::RefCell;
use std::rc::{Rc, Weak};

#[derive(Default)]
struct TrieNode {
    word_end: bool,
    children: [Option<Rc<RefCell<Self>>>; 26],
}

#[derive(Default)]
struct StreamChecker {
    root: Rc<RefCell<TrieNode>>,
    queries: Vec<Weak<RefCell<TrieNode>>>,
}

impl StreamChecker {
    fn new(words: Vec<String>) -> Self {
        let mut stream = StreamChecker::default();

        for w in &words {
            let mut node_rc = &stream.root.clone();

            for (i, c) in w.chars().enumerate() {
                let child_index = (c as u32 - 'a' as u32) as usize;
                let next_rc = node_rc.borrow_mut().children[child_index]
                                                  .get_or_insert_with(Default::default).clone();
                node_rc = next_rc;
            }

            node_rc.borrow_mut().word_end = true;
        }

        stream
    }

    fn query(&mut self, letter: char) -> bool {
        let child_index = (letter as u32 - 'a' as u32) as usize;
        let mut word_end = false;

        self.queries.push(Rc::downgrade(&self.root));

        for i in (0..self.queries.len()).rev() {
            if let Some(node_rc) = self.queries[i].upgrade() {
                if let Some(ref child_rc) = &node_rc.borrow().children[child_index] {
                    word_end |= child_rc.borrow().word_end;
                    self.queries[i] = Rc::downgrade(child_rc);
                    continue;
                }
            }

            self.queries.swap_remove(i); // no child node for this letter; remove this query
        }

        word_end
    }
}

/**
 * Your StreamChecker object will be instantiated and called as such:
 * let obj = StreamChecker::new(words);
 * let ret_1: bool = obj.query(letter);
 */
