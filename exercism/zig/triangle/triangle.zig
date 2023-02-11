pub const TriangleError = error{ InvalidInequality, Degenerate };

pub const Triangle = struct {
    ty: enum { equilateral, isosceles, scalene },

    pub fn init(a: f64, b: f64, c: f64) TriangleError!Triangle {
        if (a <= 0 and b <= 0 and c <= 0) return error.Degenerate;
        if (a == b and b == c) return .{ .ty = .equilateral };
        if (a > b + c or b > a + c or c > a + b) return error.InvalidInequality;
        return .{ .ty = if (a == b or b == c or c == a) .isosceles else .scalene };
    }

    pub fn isEquilateral(self: Triangle) bool {
        return self.ty == .equilateral;
    }

    pub fn isIsosceles(self: Triangle) bool {
        return self.ty != .scalene;
    }

    pub fn isScalene(self: Triangle) bool {
        return self.ty == .scalene;
    }
};
