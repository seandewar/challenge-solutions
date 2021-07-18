// https://leetcode.com/problems/three-equal-parts
//
// This has room for optimization and clean-up, but I'm too tired rn. :^)
// Complexity: runtime O(could be better), space O(1). :^)

impl Solution {
    pub fn three_equal_parts(arr: Vec<i32>) -> Vec<i32> {
        let mut ones_rem = arr.iter().filter(|&&x| x == 1).count();
        if ones_rem == 0 {
            return vec![0, 2];
        } else if ones_rem % 3 != 0 {
            return vec![-1, -1];
        }

        let mut ones = 0;
        for (i, &x) in arr.iter().enumerate() {
            if x == 1 {
                ones += 1;
                ones_rem -= 1;
            }
            if ones_rem == ones * 2 {
                let (a, b) = arr.split_at(i + 1);
                let a_leading = a.iter().position(|&x| x == 1).unwrap_or(a.len());
                let b_leading = b.iter().position(|&x| x == 1).unwrap_or(b.len());

                let (a, b) = (&a[a_leading..], &b[b_leading..]);
                if a.iter().eq(b.iter().take(a.len()))
                    && b[a.len()..].iter().skip_while(|&&x| x == 0).eq(a.iter())
                {
                    return vec![i as _, (i + b_leading + a.len() + 1) as _];
                }
            }
        }

        vec![-1, -1]
    }
}
