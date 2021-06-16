// https://leetcode.com/problems/matchsticks-to-square
//
// Tracking the length of only the side we're currently filling.
// Complexity: runtime O(n), space O(n) [recursion overhead].

impl Solution {
    pub fn makesquare(mut matchsticks: Vec<i32>) -> bool {
        let perim: i32 = matchsticks.iter().sum();
        perim % 4 == 0 && Self::solve(&mut matchsticks[..], 0, 0, 1, perim / 4)
    }

    fn solve(sticks: &mut [i32], l: usize, len: i32, side: u8, max: i32) -> bool {
        side == 4
            || (len == max && Self::solve(sticks, 0, 0, side + 1, max))
            || (l..sticks.len()).any(|i| {
                let stick = std::mem::take(&mut sticks[i]);
                let res = stick != 0 && Self::solve(sticks, i + 1, len + stick, side, max);
                sticks[i] = stick;
                res
            })
    }
}

// Alternative Solution: Tracking the length of each side. Slow; barely passes.
// Complexity: runtime O(4^n), space O(n) [recursion overhead].

impl Solution {
    pub fn makesquare(matchsticks: Vec<i32>) -> bool {
        let mut sides = [0; 4];
        let perim = matchsticks.iter().sum::<i32>();
        perim % 4 == 0 && Self::solve(&matchsticks[..], &mut sides, perim / 4)
    }

    fn solve(matchsticks: &[i32], sides: &mut [i32; 4], max: i32) -> bool {
        if sides.iter().any(|&l| l > max) {
            false
        } else if matchsticks.is_empty() {
            sides.iter().all(|&l| l == max)
        } else {
            let stick = matchsticks[0];
            (0..4).any(|i| {
                sides[i] += stick;
                let res = Self::solve(&matchsticks[1..], sides, max);
                sides[i] -= stick;
                res
            })
        }
    }
}
