// https://leetcode.com/problems/design-underground-system
//
// Complexity: runtime all average O(1), space O(n+r) [both ignoring String copies and data size;
//             where n == number of people checked-in, r == number of different routes.

use std::collections::HashMap;

#[derive(Default)]
struct UndergroundSystem {
    travelling: HashMap<i32, (String, i32)>,
    avg_times: HashMap<(String, String), (f64, usize)>,
}

impl UndergroundSystem {
    fn new() -> Self {
        Default::default()
    }

    fn check_in(&mut self, id: i32, station: String, t: i32) {
        assert!(self.travelling.insert(id, (station, t)).is_none());
    }

    fn check_out(&mut self, id: i32, station: String, t: i32) {
        let (from, t_from) = self.travelling.remove(&id).unwrap();
        let entry = self.avg_times.entry((from, station)).or_default();
        let (t_avg, count) = *entry;
        let (t_diff, countf) = ((t - t_from) as f64, count as f64);
        *entry = ((t_avg * countf + t_diff) / (countf + 1.), count + 1)
    }

    fn get_average_time(&self, start: String, end: String) -> f64 {
        self.avg_times[&(start, end)].0
    }
}
