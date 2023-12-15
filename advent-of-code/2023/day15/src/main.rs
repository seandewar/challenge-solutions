use std::io::{self, Write};

// Vec ought to perform better than a linked list for part 2 (and in many situations), despite
// arbitrary removals within a bucket (box). We could even consider other collision resolution
// strategies here, but this is simple enough!
type Box<'a> = Vec<(&'a [u8], u8)>;
const EMPTY_BOX: Box = Vec::new();

fn main() {
    let input = std::fs::read("input").unwrap();
    let input_it = input
        .split(|&c| c == b',')
        .map(|s| {
            // In typical Rust fashion, `trim_ascii()` and friends are unstable. I'd rather not have
            // to convert this into a UTF-8 string type just to `trim()`... lmao
            let non_ws = |c: &u8| !c.is_ascii_whitespace();
            let i = s.iter().position(non_ws).unwrap_or(s.len());
            let j = s.iter().rposition(non_ws).unwrap_or(s.len());
            &s[i..=j]
        })
        .filter(|s| !s.is_empty());

    let hasher = |key: &[u8]| -> u8 {
        key.iter()
            .fold(0, |acc, &c| acc.wrapping_add(c).wrapping_mul(17))
    };
    print!(
        "Day15: P1: {}",
        input_it.clone().map(|s| u32::from(hasher(s))).sum::<u32>()
    );
    io::stdout().flush().unwrap();

    let mut hm = [EMPTY_BOX; 256];
    let find_lense = |hm: &[Box; 256], key: &[u8]| -> (usize, Option<usize>) {
        let box_i = usize::from(hasher(key));
        (box_i, hm[box_i].iter().position(|&(k, _)| k == key))
    };

    for cmd in input_it {
        if let Some(i) = cmd.iter().position(|&c| c == b'=') {
            let (key, suffix) = cmd.split_at(i);
            assert!(suffix.len() == 2 && suffix[1].is_ascii_digit());
            let lense @ (key, focal) = (key, suffix[1] - b'0');
            match find_lense(&hm, key) {
                (box_i, Some(lense_i)) => hm[box_i][lense_i].1 = focal,
                (box_i, None) => hm[box_i].push(lense),
            }
        } else {
            let (minus, key) = cmd.split_last().unwrap();
            assert!(*minus == b'-');
            if let (box_i, Some(lense_i)) = find_lense(&hm, key) {
                hm[box_i].remove(lense_i);
            }
        }
    }
    println!(
        ", P2: {}",
        hm.iter()
            .zip(1..)
            .filter(|(lenses, _)| !lenses.is_empty())
            .map(|(lenses, box_num)| {
                lenses
                    .iter()
                    .zip(1..)
                    .map(|(&(_, focal), lense_num)| box_num * lense_num * u32::from(focal))
                    .sum::<u32>()
            })
            .sum::<u32>(),
    );
}
