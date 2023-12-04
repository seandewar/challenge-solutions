package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
	"strings"
)

func main() {
	f, err := os.Open("input")
	assert(err == nil)
	defer f.Close()

	p1, p2 := 0, 0
	for i, copies, scanner := 0, []int{}, bufio.NewScanner(f); scanner.Scan(); i++ {
		fields := strings.Fields(scanner.Text())
		assert(fields[0] == "Card")

		numStr, found := strings.CutSuffix(fields[1], ":")
		assert(found)
		_, err := strconv.ParseUint(numStr, 10, 32)
		assert(err == nil)

		scratched, fields := [100]bool{}, fields[2:]
		for i, numStr := range fields {
			if numStr == "|" {
				fields = fields[i+1:]
				break
			}

			num, err := strconv.ParseUint(numStr, 10, 32)
			assert(err == nil)
			scratched[num] = true
		}

		matches, points := 0, 0
		for _, numStr := range fields {
			num, err := strconv.ParseUint(numStr, 10, 32)
			assert(err == nil)
			if !scratched[num] {
				continue
			}

			matches++
			if points == 0 {
				points = 1
			} else {
				points *= 2
			}
		}

		// A ring buffer is better, as we don't need to remember the counts of all cards,
		// but I am lazy.
		if i == len(copies) {
			copies = append(copies, 1)
		}
		for j := i + 1; j <= i+matches; j++ {
			if j == len(copies) {
				copies = append(copies, 1)
			}
			copies[j] += copies[i]
		}

		p1 += points
		p2 += copies[i]
	}

	fmt.Println("Day4: P1:", p1, "P2:", p2)
}

func assert(cond bool) {
	if !cond {
		panic("assertion failed")
	}
}
