// https://leetcode.com/problems/generate-random-point-in-a-circle
//
// Rejection Sampling.
// Complexity: runtime average O(1) all, rand_point() worst case O(infinity), space O(1).

use rand::{
    distributions::{Distribution, Uniform},
    Rng,
};

struct Solution {
    x_center: f64,
    y_center: f64,
    radius: f64,
}

impl Solution {
    fn new(radius: f64, x_center: f64, y_center: f64) -> Self {
        Self {
            radius,
            x_center,
            y_center,
        }
    }

    fn rand_point(&self) -> Vec<f64> {
        let mut rng = rand::thread_rng();
        let range = Uniform::from(-self.radius..self.radius);

        loop {
            let (x, y) = (range.sample(&mut rng), range.sample(&mut rng));
            if x.powi(2) + y.powi(2) < self.radius.powi(2) {
                break vec![self.x_center + x, self.y_center + y];
            }
        }
    }
}
