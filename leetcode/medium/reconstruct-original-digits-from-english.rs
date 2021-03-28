// https://leetcode.com/problems/reconstruct-original-digits-from-english
//
// Complexity: runtime O(n), space O(1) [O(n) including output].

static UNIQ_MAP: [(&str, usize); 10] = [
    ("zero", 0),
    ("two", 2),
    ("four", 4),
    ("six", 6),
    ("eight", 8),
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
