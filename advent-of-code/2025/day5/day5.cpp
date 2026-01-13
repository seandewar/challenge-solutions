#include <algorithm>
#include <charconv>
#include <cstdint>
#include <fstream>
#include <print>
#include <ranges>
#include <string>
#include <utility>
#include <vector>

int main()
{
    std::ifstream file("input");
    if (!file) {
        std::abort();
    }

    std::vector<std::pair<std::uint64_t, std::uint64_t>> ranges;

    std::string line;
    while (std::getline(file, line) && !line.empty()) {
        const std::size_t dash_pos = line.find('-');
        if (dash_pos == std::string::npos) {
            std::abort();
        }
        std::uint64_t min = 0;
        const auto [p1, ec1] =
            std::from_chars(line.data(), line.data() + dash_pos, min);
        if (ec1 != std::errc() || p1 != line.data() + dash_pos) {
            std::abort();
        }
        std::uint64_t max = 0;
        const auto [p2, ec2] = std::from_chars(line.data() + dash_pos + 1,
                                               line.data() + line.size(), max);
        if (ec2 != std::errc() || p2 != line.data() + line.size()) {
            std::abort();
        }

        ranges.emplace_back(min, max);
    }
    if (file.eof()) {
        std::abort();
    }

    // Sort and merge intervals.
    const auto lt_left = [](const auto &a, const auto &b) -> bool {
        return a.first < b.first;
    };
    std::ranges::sort(ranges, lt_left);
    std::size_t i = 0;
    for (const auto &[l, r] : ranges | std::views::drop(1)) {
        if (l <= ranges[i].second) {
            ranges[i].second = std::max(ranges[i].second, r);
        } else {
            ranges[++i] = {l, r};
        }
    }
    ranges.resize(i + 1);

    std::size_t p1 = 0;
    while (std::getline(file, line)) {
        std::uint64_t query = 0;
        const auto [p, ec] =
            std::from_chars(line.data(), line.data() + line.size(), query);
        if (ec != std::errc() && p != line.data() + line.size()) {
            std::abort();
        }

        auto it =
            std::ranges::upper_bound(ranges, std::pair{query, query}, lt_left);
        if (it-- != ranges.begin() && it->first <= query
            && query <= it->second) {
            ++p1;
        }
    }

    std::println("Day5: P1: {}, P2: {}", p1,
                 std::ranges::fold_left(
                     ranges, 0, [](const auto acc, const auto &range) {
                         return acc + (range.second - range.first + 1);
                     }));
}
