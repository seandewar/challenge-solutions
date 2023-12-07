package main

import "core:fmt"
import "core:os"
import "core:strconv"
import "core:strings"

Race :: struct {time, dist: uint}

main :: proc() {
    p1_races, p2_race, ok := parse_file("input")
    assert(ok)

    compute_wins :: proc(race: Race) -> (wins: int) {
        for speed in 1..<race.time {
            if time_left := race.time - speed; time_left * speed > race.dist {
                wins += 1
            }
        }
        return
    }

    p1 := len(p1_races) > 0 ? 1 : 0
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
