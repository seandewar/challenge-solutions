pub fn primes(buffer: []u32, limit: u32) []u32 {
    var sieve = [_]bool{false} ** 1001;
    var num: u16 = 3;
    while (num <= @import("std").math.sqrt(limit)) : (num += 2) {
        if (sieve[num]) continue;
        var mark = num * 2;
        while (mark <= limit) : (mark += num) sieve[mark] = true;
    }
    num = 3;
    var prime_count: u16 = 0;
    while (num <= limit) : (num += 2) if (!sieve[num]) {
        buffer[prime_count] = num;
        prime_count += 1;
    };
    return buffer[0..prime_count];
}
