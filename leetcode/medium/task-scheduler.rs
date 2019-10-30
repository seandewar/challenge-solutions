// https://leetcode.com/problems/task-scheduler/
//
// Same as the C++ solution, but in Rust. See task-scheduler.cpp for comments.
// Complexity: runtime O(n), space O(1).

impl Solution {
    pub fn least_interval(tasks: Vec<char>, n: i32) -> i32 {
        let mut task_counts = [0usize; 26];
        for t in tasks {
            task_counts[t as usize - 'A' as usize] += 1;
        }

        let compare_desc = |a: &usize, b: &usize| b.cmp(a);
        task_counts.sort_unstable_by(compare_desc);

        let mut result = 0;

        while task_counts[0] > 0 {
            let mut i = 0usize;

            while i < task_counts.len() && task_counts[i] > 0 {
                if i > n as usize {
                    task_counts.sort_unstable_by(compare_desc);
                    break;
                }

                task_counts[i] -= 1;
                i += 1;
            }

            result += if i <= n as usize && task_counts[0] > 0 {
                n + 1
            } else {
                i as i32
            }
        }

        result
    }
}
