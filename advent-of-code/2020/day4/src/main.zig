///! Bram-tier code, but completely comptime!
const std = @import("std");

const Field = enum {
    byr,
    iyr,
    eyr,
    hgt,
    hcl,
    ecl,
    pid,
    cid,

    pub fn fromName(name: []const u8) Field {
        // Could use @typeInfo() to get the field names, but this works (and is maybe faster).
        return switch (name[0]) {
            'b' => .byr,
            'i' => .iyr,
            'p' => .pid,
            'c' => .cid,
            'e' => switch (name[1]) {
                'y' => .eyr,
                'c' => .ecl,
                else => unreachable,
            },
            'h' => switch (name[1]) {
                'g' => .hgt,
                'c' => .hcl,
                else => unreachable,
            },
            else => unreachable,
        };
    }
};

fn yearIsValid(input: []const u8, min: u16, max: u16) bool {
    const year = std.fmt.parseUnsigned(u16, input, 10) catch return false;
    return year >= min and year <= max;
}

pub fn main() !void {
    const input = @embedFile("input.txt");
    const results = comptime blk: {
        @setEvalBranchQuota(500_000);
        var day1_result = 0;
        var day2_result = 0;

        var i = 0;
        while (i < input.len) {
            var seen_fields = [_]bool{false} ** @typeInfo(Field).Enum.fields.len;
            var day1_valid = true;
            var day2_valid = true;

            while (std.mem.indexOfAnyPos(u8, input, i, &.{ ':', '\n' })) |j| {
                const start_i = j + 1;
                const end_i = std.mem.indexOfAnyPos(u8, input, start_i, &.{ ' ', '\n' }) orelse input.len;
                const value_str = input[start_i..end_i];

                switch (input[j]) {
                    ':' => {
                        const field = Field.fromName(input[j - 3 .. j]);
                        const field_i = @enumToInt(field);
                        if (seen_fields[field_i]) {
                            day1_valid = false;
                        } else {
                            seen_fields[field_i] = true;
                        }

                        if (day2_valid) {
                            day2_valid = switch (field) {
                                .byr => yearIsValid(value_str, 1920, 2002),
                                .iyr => yearIsValid(value_str, 2010, 2020),
                                .eyr => yearIsValid(value_str, 2020, 2030),
                                .hgt => case_blk: {
                                    if (value_str.len < 2) {
                                        break :case_blk false;
                                    }
                                    const unit = value_str[value_str.len - 2 ..];
                                    if (!std.mem.eql(u8, unit, "cm") and !std.mem.eql(u8, unit, "in")) {
                                        break :case_blk false;
                                    }

                                    const value = std.fmt.parseUnsigned(u8, value_str[0 .. value_str.len - 2], 10) catch break :case_blk false;
                                    break :case_blk switch (unit[0]) {
                                        'c' => value >= 150 and value <= 193,
                                        'i' => value >= 59 and value <= 76,
                                        else => unreachable,
                                    };
                                },
                                .hcl => case_blk: {
                                    if (value_str.len != 7 or value_str[0] != '#') {
                                        break :case_blk false;
                                    }
                                    for (value_str[1..]) |c| {
                                        if ((c < '0' or c > '9') and (c < 'a' or c > 'f')) {
                                            break :case_blk false;
                                        }
                                    }
                                    break :case_blk true;
                                },
                                .ecl => case_blk: {
                                    for (.{ "amb", "blu", "brn", "gry", "grn", "hzl", "oth" }) |valid_value| {
                                        if (std.mem.eql(u8, value_str, valid_value)) {
                                            break :case_blk true;
                                        }
                                    }
                                    break :case_blk false;
                                },
                                .pid => case_blk: {
                                    if (value_str.len != 9) {
                                        break :case_blk false;
                                    }
                                    for (value_str) |c| {
                                        if (c < '0' or c > '9') {
                                            break :case_blk false;
                                        }
                                    }
                                    break :case_blk true;
                                },
                                .cid => true, // Ignored
                            };
                        }
                    },
                    '\n' => {
                        if (start_i < input.len and input[start_i] == '\n') {
                            i = start_i + 1;
                            break;
                        }
                    },
                    else => unreachable,
                }
                i = start_i;
            }
            if (!day1_valid) {
                continue;
            }

            for (seen_fields) |seen, field_i| {
                if (field_i != @enumToInt(Field.cid) and !seen) {
                    day1_valid = false;
                    break;
                }
            }
            if (day1_valid) {
                day1_result += 1;
                if (day2_valid) {
                    day2_result += 1;
                }
            }
        }

        break :blk .{ day1_result, day2_result };
    };

    const stdout = std.io.getStdOut().writer();
    try stdout.print("Day 4\nP1: {d}\nP2: {d}\n", results);
}
