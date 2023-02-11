const mem = @import("std").mem;

pub const RnaError = error{IllegalDnaNucleotide};

pub fn toRna(allocator: mem.Allocator, dna: []const u8) (RnaError || mem.Allocator.Error)![]const u8 {
    const s = try allocator.alloc(u8, dna.len);
    errdefer allocator.free(s);
    for (s) |*c, i| c.* = switch (dna[i]) {
        'G' => 'C',
        'C' => 'G',
        'T' => 'A',
        'A' => 'U',
        else => return error.IllegalDnaNucleotide,
    };
    return s;
}
