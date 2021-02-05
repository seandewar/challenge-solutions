// https://leetcode.com/problems/design-parking-system
//
// Complexity: runtime O(1), space O(1) for all operations.

struct ParkingSystem {
    big_spaces: i32,
    medium_spaces: i32,
    small_spaces: i32,
}

impl ParkingSystem {
    fn new(big: i32, medium: i32, small: i32) -> Self {
        Self {
            big_spaces: big,
            medium_spaces: medium,
            small_spaces: small,
        }
    }

    fn add_car(&mut self, car_type: i32) -> bool {
        let spaces = match car_type {
            1 => &mut self.big_spaces,
            2 => &mut self.medium_spaces,
            3 => &mut self.small_spaces,
            _ => unreachable!("test inputs guarantee a valid car_type"),
        };

        *spaces -= 1; // can decrement into -ves; doesn't matter for this problem :)
        *spaces >= 0
    }
}

// Alternative Solution: Shortest version.
// Complexity: runtime O(1), space O(1) for all operations.

struct ParkingSystem {
    spaces: [i32; 3],
}

impl ParkingSystem {
    fn new(big: i32, medium: i32, small: i32) -> Self {
        Self {
            spaces: [big, medium, small],
        }
    }

    fn add_car(&mut self, car_type: i32) -> bool {
        self.spaces[car_type as usize - 1] -= 1;
        self.spaces[car_type as usize - 1] >= 0
    }
}
