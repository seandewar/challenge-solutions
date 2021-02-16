// https://leetcode.com/problems/subrectangle-queries
//
// Alternatively, you can just use a Vec<i32> (size rows * cols) or a Vec<Vec<i32>> for mat.
// If you use a Vec<Vec<i32>>, new() can just move the rectangle Vec into mat. Indexing is also
// syntactically easier. None of this really matter for this problem.
//
// Complexity: runtime new(): O(100*100) == O(1), update_subrectangle(): O(100*100) == O(1),
// get_value(): O(1); space O(100*100) == O(1).

/**
 * Your SubrectangleQueries object will be instantiated and called as such:
 * let obj = SubrectangleQueries::new(rectangle);
 * obj.update_subrectangle(row1, col1, row2, col2, newValue);
 * let ret_2: i32 = obj.get_value(row, col);
 */
struct SubrectangleQueries {
    mat: [i32; 100 * 100],
}

impl SubrectangleQueries {
    fn new(rectangle: Vec<Vec<i32>>) -> Self {
        let mut ret = Self {
            mat: [0; 100 * 100],
        };
        for (y, row) in rectangle.into_iter().enumerate() {
            for (x, val) in row.into_iter().enumerate() {
                ret.mat[y * 100 + x] = val;
            }
        }

        ret
    }

    fn update_subrectangle(&mut self, row1: i32, col1: i32, row2: i32, col2: i32, new_value: i32) {
        for y in row1..=row2 {
            for x in col1..=col2 {
                self.mat[y as usize * 100 + x as usize] = new_value;
            }
        }
    }

    fn get_value(&self, row: i32, col: i32) -> i32 {
        self.mat[row as usize * 100 + col as usize]
    }
}
