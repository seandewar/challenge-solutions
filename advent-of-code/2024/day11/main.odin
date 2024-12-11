package main

import "core:fmt"
import "core:os"
import "core:strconv"
import "core:strings"

Stone_Memo_Key :: struct {
    value: uint,
    n_blinks: uint,
}
Stone_Memo :: map[Stone_Memo_Key]uint

count_stones :: proc(memo: ^Stone_Memo, value: uint, n_blinks: uint) -> uint {
    if n_blinks == 0 {
        return 1
    }
    memo_key := Stone_Memo_Key{value, n_blinks}
    ret, memo_ok := memo[memo_key]
    if memo_ok {
        return ret
    }

    n_digits: uint
    for x := value; x > 0; x /= 10 do n_digits += 1

    switch {
    case value == 0:
        ret = count_stones(memo, 1, n_blinks - 1)

    case n_digits % 2 == 0:
        l, r, r_mul: uint = value, 0, 1
        for _ in 0..<n_digits / 2 {
            r += (l % 10) * r_mul
            r_mul *= 10
            l /= 10
        }
        ret = count_stones(memo, l, n_blinks - 1) +
              count_stones(memo, r, n_blinks - 1)

    case:
        ret = count_stones(memo, value * 2024, n_blinks - 1)
    }

    memo[memo_key] = ret
    return ret
}

main :: proc() {
    context.allocator = context.temp_allocator

    input, input_ok := os.read_entire_file("input")
    if !input_ok {
        panic("failed to read input file")
    }

    p1, p2: uint
    memo := make(Stone_Memo)
    field_it := string(input)
    for field in strings.fields_iterator(&field_it) {
        value, value_ok := strconv.parse_uint(field, 10)
        if !value_ok {
            panic("malformed input")
        }

        p1 += count_stones(&memo, value, 25);
        p2 += count_stones(&memo, value, 75);
    }

    fmt.printf("Day11: P1: %d, P2: %d\n", p1, p2)
}
