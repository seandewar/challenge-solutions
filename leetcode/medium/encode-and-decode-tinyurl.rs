// https://leetcode.com/problems/encode-and-decode-tinyurl
//
// This is a more "production-ready" solution.
// This generates a random **unique** short URL for each URL given.
//
// If a shortUrl is generated for a longUrl that is already mapped, we try again until this isn't
// the case.
//
// Of course, it's very unlikely that this happens; there's `(26 + 26 + 10) ^ 6 == 56 800 235 584`
// possible shortUrls we can generate here.
//
// Complexity: runtime: decode() average O(s), encode() average O(s) [worst case O(infinity)]
//             space O(t*n).
//             [where s == input string len(), t == average strings len() in maps, n == num of
//             strings in maps]

use rand::{distributions::Alphanumeric, Rng};
use std::{
    cell::RefCell,
    collections::{hash_map::Entry, HashMap},
};

#[derive(Default)]
struct Codec {
    to_long: RefCell<HashMap<String, String>>,
    to_short: RefCell<HashMap<String, String>>,
}

impl Codec {
    fn new() -> Self {
        Default::default()
    }

    fn encode(&self, longUrl: String) -> String {
        let (mut to_short, mut to_long) = (self.to_short.borrow_mut(), self.to_long.borrow_mut());

        match to_short.entry(longUrl.clone()) {
            Entry::Occupied(o) => o.get().clone(),
            Entry::Vacant(v_short) => loop {
                let shortUrl: String = "https://tinyurl.com/"
                    .chars()
                    .chain(rand::thread_rng().sample_iter(&Alphanumeric).take(6))
                    .collect();

                if let Entry::Vacant(v_long) = to_long.entry(shortUrl.clone()) {
                    v_long.insert(longUrl);
                    break v_short.insert(shortUrl).clone();
                }
            },
        }
    }

    fn decode(&self, shortUrl: String) -> String {
        self.to_long.borrow()[&shortUrl].clone()
    }
}
