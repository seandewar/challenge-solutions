use std::{
    collections::HashMap,
    sync::{
        atomic::{AtomicUsize, Ordering},
        Mutex,
    },
    thread,
};

pub fn frequency(input: &[&str], worker_count: usize) -> HashMap<char, usize> {
    let freqs = Mutex::new(HashMap::new());
    let next_i = AtomicUsize::new(0);

    thread::scope(|s| {
        for _ in 0..worker_count {
            s.spawn(|| {
                let mut curr_freqs = HashMap::new();
                loop {
                    let i = next_i.fetch_add(1, Ordering::Relaxed);
                    if i >= input.len() {
                        break;
                    }

                    for c in input[i]
                        .chars()
                        .filter(|c| c.is_alphabetic())
                        .map(|c| c.to_ascii_lowercase())
                    {
                        *curr_freqs.entry(c).or_insert(0) += 1;
                    }
                }

                let mut freqs = freqs.lock().unwrap();
                for (&c, &n) in curr_freqs.iter() {
                    *freqs.entry(c).or_insert(0) += n;
                }
            });
        }
    });

    freqs.into_inner().unwrap()
}
