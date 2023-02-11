pub const SpaceAge = struct {
    seconds: isize,

    pub fn init(seconds: isize) SpaceAge {
        return .{ .seconds = seconds };
    }

    pub fn onMercury(self: SpaceAge) f64 {
        return self.onEarth() / 0.2408467;
    }

    pub fn onVenus(self: SpaceAge) f64 {
        return self.onEarth() / 0.61519726;
    }

    pub fn onEarth(self: SpaceAge) f64 {
        return @intToFloat(f64, self.seconds) / 31557600.0;
    }

    pub fn onMars(self: SpaceAge) f64 {
        return self.onEarth() / 1.8808158;
    }

    pub fn onJupiter(self: SpaceAge) f64 {
        return self.onEarth() / 11.862615;
    }

    pub fn onSaturn(self: SpaceAge) f64 {
        return self.onEarth() / 29.447498;
    }

    pub fn onUranus(self: SpaceAge) f64 {
        return self.onEarth() / 84.016846;
    }

    pub fn onNeptune(self: SpaceAge) f64 {
        return self.onEarth() / 164.79132;
    }
};
