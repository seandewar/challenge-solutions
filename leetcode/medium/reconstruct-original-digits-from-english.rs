// https://leetcode.com/problems/reconstruct-original-digits-from-english
//
// Complexity: runtime O(n), space O(1) [O(n) including output].

static UNIQ_MAP: [(&str, usize); 10] = [
    ("zero", 0),
    ("wto", 2),
    ("ufor", 4),
    ("xsi", 6),
    ("geiht", 8),
    ("one", 1),
    ("thre", 3),
    ("five", 5),
    ("sevn", 7),
    ("nie", 9),
];

impl Solution {
    pub fn original_digits(s: String) -> String {
        let mut sc = [0; 26];
        s.chars().for_each(|c| sc[c as usize - 'a' as usize] += 1);

        let rc = UNIQ_MAP.iter().fold([0; 10], |mut a, &(w, i)| {
            let n = w
                .chars()
                .map(|c| sc[c as usize - 'a' as usize])
                .min()
                .unwrap();

            w.chars().for_each(|c| sc[c as usize - 'a' as usize] -= n);
            a[i] += n;
            a
        });

        rc.iter()
            .enumerate()
            .flat_map(|(i, &n)| std::iter::repeat((b'0' + i as u8) as char).take(n))
            .collect()
    }
}

// Alternative Solution: Slightly more efficient and simplified version of the above.
// Complexity: runtime O(n), space O(1) [O(n) including output].

use std::iter::repeat;

impl Solution {
    pub fn original_digits(s: String) -> String {
        let mut sc = [0; 26];
        s.chars().for_each(|c| sc[c as usize - 'a' as usize] += 1);
        let sc = |c| sc[c as usize - 'a' as usize];

        (repeat('0').take(sc('z')))
            .chain(repeat('1').take(sc('o') - sc('z') - sc('w') - sc('u')))
            .chain(repeat('2').take(sc('w')))
            .chain(repeat('3').take(sc('h') - sc('g')))
            .chain(repeat('4').take(sc('u')))
            .chain(repeat('5').take(sc('f') - sc('u')))
            .chain(repeat('6').take(sc('x')))
            .chain(repeat('7').take(sc('v') - sc('f') + sc('u')))
            .chain(repeat('8').take(sc('g')))
            .chain(repeat('9').take(sc('i') - sc('x') - sc('g') - sc('f') + sc('u')))
            .collect()
    }
}
