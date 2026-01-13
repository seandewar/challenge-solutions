#include <algorithm>
#include <cstdint>
#include <fstream>
#include <print>
#include <ranges>
#include <string>

int main()
{
    std::ifstream file("input");
    if (!file.is_open()) {
        std::abort();
    }

    std::uint64_t p1 = 0;
    std::uint64_t p2 = 0;

    std::string line;
    while (std::getline(file, line)) {
        if (line.size() < 12) {
            std::abort();
        }

        // ASCII order is the same as numeric order, so this is fine.
        const auto max_left_it = std::max_element(line.begin(), line.end() - 1);
        const auto max_right_it = std::max_element(max_left_it + 1, line.end());
        p1 += (*max_left_it - '0') * 10 + (*max_right_it - '0');

        std::uint64_t p2_joltage = 0;
        auto start_it = line.begin();
        for (unsigned i = 0; i < 12; ++i) {
            const auto max_it = std::max_element(start_it, line.end() - 11 + i);
            p2_joltage = p2_joltage * 10 + (*max_it - '0');
            start_it = max_it + 1;
        }

        p2 += p2_joltage;
    }

    std::println("Day3: P1: {}, P2: {}", p1, p2);
}
