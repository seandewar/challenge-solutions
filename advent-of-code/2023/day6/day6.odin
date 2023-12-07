package main

import "core:fmt"
import "core:math"
import "core:os"
import "core:strconv"
import "core:strings"

Race :: struct {time, dist: uint}

main :: proc() {
    p1_races, p2_race, ok := parse_file("input")
    assert(ok)
    defer delete(p1_races)

    compute_wins :: proc(race: Race) -> uint {
        // (t - x) * x > d  =>  -x^2 + tx - d > 0  [a = -1, b = t, c = -d - 1]
        b, c := int(race.time), -int(race.dist)
        d := b * b + 4 * c
        if d <= 0 { // No roots (can't win) or one root (draw at best)
            return 0
        }

        d_sqrt := math.sqrt(f64(d))
        r0, r1 := (f64(b) - d_sqrt) / 2, (f64(b) + d_sqrt) / 2
        // Want the length of the interval where quadratic > 0, but only for integer values of x;
        // ceil(r0) or floor(r1) if they are fractional, but otherwise r0+1 or r1-1 if they are
        // integral, which is what the floor(r0+1) and ceil(r1-1) logic achieves below
        return uint(math.ceil(r1 - 1) - math.floor(r0 + 1)) + 1
    }

    p1: uint = len(p1_races) > 0 ? 1 : 0
    for race in p1_races {
        p1 *= compute_wins(race)
    }

    fmt.println("Day6: P1:", p1, "P2:", compute_wins(p2_race))
}

parse_file :: proc(name: string, allocator := context.allocator) -> (p1_races: [dynamic]Race, p2_race: Race, ok: bool) {
    context.allocator = allocator

    input := os.read_entire_file(name) or_return
    defer delete(input)

    field_it := string(input)
    if field := strings.fields_iterator(&field_it) or_return; field != "Time:" {
        return
    }

    append_digits :: proc(lhs: uint, rhs: uint) -> uint {
        base := lhs
        for n := rhs; n > 0; n /= 10 {
            base *= 10
        }
        return base + rhs
    }

    for field in strings.fields_iterator(&field_it) {
        if field == "Distance:" {
            break
        }

        time := strconv.parse_uint(field, 10) or_return
        append(&p1_races, Race{time = time})
        p2_race.time = append_digits(p2_race.time, time)
    }

    for _, i in p1_races {
        field := strings.fields_iterator(&field_it) or_return
        dist := strconv.parse_uint(field, 10) or_return
        p1_races[i].dist = dist
        p2_race.dist = append_digits(p2_race.dist, dist)
    }

    _, not_eof := strings.fields_iterator(&field_it)
    return p1_races, p2_race, !not_eof
}
