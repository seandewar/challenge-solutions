// https://leetcode.com/problems/stamping-the-sequence
//
// Complexity: runtime O(t^2) [target = "aaaaaaab", stamp = "ab"; this can be mitigated by
//                             reversing iteration direction for "i" each pass],
//             space O(1) ignoring output [O(t) otherwise];
//             where t == target.len().

impl Solution {
    pub fn moves_to_stamp(stamp: String, target: String) -> Vec<i32> {
        let (stamp, mut target) = (stamp.as_bytes(), target.into_bytes());
        let mut res = Vec::new();

        while target.iter().any(|&c| c != b'?') {
            let prev_res_len = res.len();

            for i in 0..target.len() - stamp.len() + 1 {
                let sub = &mut target[i..i + stamp.len()];

                if sub.iter().any(|&c| c != b'?')
                    && sub
                        .iter()
                        .zip(stamp.iter())
                        .all(|(&wc, &sc)| wc == sc || wc == b'?')
                {
                    sub.iter_mut().for_each(|c| *c = b'?');
                    res.push(i as _);
                }
            }
            if res.len() == prev_res_len {
                return Vec::new();
            }
        }

        res.reverse();
        res
    }
}
