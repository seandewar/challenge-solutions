# Advent of Code 2022 in Zig

Includes solutions to all [Advent of Code 2022](https://adventofcode.com/2022)
problems, written in [Zig](https://ziglang.org/) and computed at compile time
where possible (barring solutions that trigger [compiler OOM issues](https://github.com/ziglang/zig/issues/12493)
and other problems; my laptop only has 8GB of RAM, and I `ulimit`ed the compiler
to 3GB to avoid the OOM-reaper murdering every _other_ process). All
runtime-based solutions still at least parse the inputs at compile time, and
most of them use no heap allocations.

Additionally, my [Day 22 Part II](day22/src/main.zig) solution should work for
any given cube net, and one-or-two extra solutions written in C and Rust are
included for fun.

Final thoughts on Zig: I love it! I look forward to seeing how this language
develops in the future. :-)
