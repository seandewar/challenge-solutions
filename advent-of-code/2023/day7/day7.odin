package main

import "core:bufio"
import "core:fmt"
import "core:io"
import "core:os"
import "core:runtime"
import "core:slice"
import "core:strconv"
import "core:strings"

p1_cards_by_rank := [?]u8{'A', 'K', 'Q', 'J', 'T', '9', '8', '7', '6', '5', '4', '3', '2'}
p2_cards_by_rank := [?]u8{'A', 'K', 'Q', 'T', '9', '8', '7', '6', '5', '4', '3', '2', 'J'}

card_order_index :: proc(c: u8, p1: bool) -> int {
    i, ok := slice.linear_search(p1 ? p1_cards_by_rank[:] : p2_cards_by_rank[:], c)
    assert(ok)
    return i
}

Hand :: struct {
    cards: []u8,
    bid: uint,
    p1_type: Hand_Type,
    p2_type: Hand_Type,
}

Hand_Type :: enum {
    Five_Of_A_Kind,
    Four_Of_A_Kind,
    Full_House,
    Three_Of_A_Kind,
    Two_Pair,
    One_Pair,
    High_Card,
}

hand_type :: proc(h: Hand, p1: bool) -> Hand_Type {
    card_counts: [len(p1_cards_by_rank)]u8
    for c in h.cards {
        card_counts[card_order_index(c, true)] += 1
    }

    joker_i := card_order_index('J', true)
    wild_count, max_count, singles_count: u8 = p1 ? 0 : card_counts[joker_i], 0, 0
    for count, i in card_counts {
        if !p1 && i == joker_i {
            continue
        }
        max_count = max(max_count, count)
        if count == 1 {
            singles_count += 1
        }
    }

    switch max_count + wild_count {
    case 5:
        return .Five_Of_A_Kind
    case 4:
        return .Four_Of_A_Kind

    case 3:
        wild_count -= 3 - max_count
        return singles_count == 0 || wild_count > 0 ? .Full_House : .Three_Of_A_Kind
    case 2:
        wild_count -= 2 - max_count
        return singles_count == 1 || wild_count > 0 ? .Two_Pair : .One_Pair

    case:
        return .High_Card
    }
}

hand_compare :: proc(l, r: Hand, p1: bool) -> slice.Ordering {
    if order := slice.cmp(p1 ? l.p1_type : l.p2_type, p1 ? r.p1_type : r.p2_type); order != .Equal {
        return order
    }
    for lc, i in l.cards {
        rc := r.cards[i]
        if lo, ro := card_order_index(lc, p1), card_order_index(rc, p1); lo != ro {
            return lo < ro ? .Less : .Greater
        }
    }
    return .Equal
}
p1_hand_compare :: proc(l, r: Hand) -> slice.Ordering { return hand_compare(l, r, true) }
p2_hand_compare :: proc(l, r: Hand) -> slice.Ordering { return hand_compare(l, r, false) }

main :: proc() {
    fd, errno := os.open("input")
    assert(errno == 0)
    defer os.close(fd)

    buf: [512]u8
    rd: bufio.Reader
    bufio.reader_init_with_buf(&rd, os.stream_from_handle(fd), buf[:])

    hands_by_rank: [dynamic]Hand
    for {
        hand, err := parse_next_line(&rd)
        assert(err == nil)
        if hand.cards == nil {
            break
        }
        append(&hands_by_rank, hand)
    }

    p1, p2: uint
    slice.reverse_sort_by_cmp(hands_by_rank[:], p1_hand_compare)
    for hand, i in hands_by_rank {
        p1 += hand.bid * uint(i + 1)
    }
    slice.reverse_sort_by_cmp(hands_by_rank[:], p2_hand_compare)
    for hand, i in hands_by_rank {
        p2 += hand.bid * uint(i + 1)
    }

    fmt.println("Day7: P1:", p1, "P2:", p2)
}

parse_next_line :: proc(rd: ^bufio.Reader) -> (hand: Hand, err: union {io.Error, runtime.Allocator_Error, bool}) {
    // Could just read the entire input and slice into it for `hand.cards`, but like yesterday I'm
    // doing more than is necessary so I can experience more Odin features.
    line, rd_err := bufio.reader_read_slice(rd, '\n')
    if rd_err == .EOF || rd_err == .No_Progress {
        return
    } else if rd_err != nil {
        return {}, rd_err
    }

    field_it := string(line)
    cards := transmute([]u8)(strings.fields_iterator(&field_it) or_return)
    card_is_invalid :: proc(c: u8) -> bool { return !slice.contains(p1_cards_by_rank[:], c) }
    if len(cards) != 5 || slice.any_of_proc(cards, card_is_invalid) {
        return {}, false
    }

    bid := strings.fields_iterator(&field_it) or_return
    hand.bid = strconv.parse_uint(bid, 10) or_return
    if _, not_eof := strings.fields_iterator(&field_it); not_eof {
        return {}, false
    }

    hand.cards = slice.clone(cards, context.temp_allocator) or_return
    hand.p1_type, hand.p2_type = hand_type(hand, true), hand_type(hand, false)
    return
}
