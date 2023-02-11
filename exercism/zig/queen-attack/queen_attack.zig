pub const QueenError = error{ InitializationFailure, SameSquare };

pub const Queen = struct {
    x: i5,
    y: i5,

    pub fn init(x: i5, y: i5) QueenError!@This() {
        if (x < 0 or y < 0 or x >= 8 or y >= 8) return error.InitializationFailure;
        return .{ .x = x, .y = y };
    }

    pub fn canAttack(self: @This(), other: @This()) QueenError!bool {
        // Use vectors for fun! Use i8 vectors, as i5 miscompiles
        // on the version of Zig that Exercism uses at the moment.
        // This bug is fixed on Zig's master branch.
        const a = @Vector(2, i8){ self.x, self.y };
        const b = @Vector(2, i8){ other.x, other.y };
        if (@reduce(.And, a == b)) return error.SameSquare;
        const d = (a - b) * @import("std").math.sign(a - b);
        return @reduce(.Or, a == b) or d[0] == d[1];
    }
};
