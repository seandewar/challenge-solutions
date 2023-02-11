pub const Coordinate = struct {
    cached_score: isize,

    pub fn init(x_coord: f32, y_coord: f32) Coordinate {
        const d_sq = x_coord * x_coord + y_coord * y_coord;
        return .{
            .cached_score = if (d_sq <= 1)
                10
            else if (d_sq <= 5 * 5)
                5
            else if (d_sq <= 10 * 10)
                1
            else
                0,
        };
    }

    pub fn score(self: Coordinate) isize {
        return self.cached_score;
    }
};
