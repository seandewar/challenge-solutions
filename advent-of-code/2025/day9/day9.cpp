#include <charconv>
#include <cstdint>
#include <fstream>
#include <print>
#include <string>
#include <utility>
#include <vector>

int main()
{
    std::ifstream f("input");
    if (!f) {
        std::abort();
    }

    std::vector<std::pair<std::int64_t, std::int64_t>> poses;

    std::string line;
    while (std::getline(f, line)) {
        std::int64_t x;
        const auto [commap, ecx] =
            std::from_chars(line.data(), line.data() + line.size(), x);
        if (ecx != std::errc() || *commap != ',') {
            std::abort();
        }

        std::int64_t y;
        const auto [endp, ecy] =
            std::from_chars(commap + 1, line.data() + line.size(), y);
        if (ecy != std::errc() || *endp != '\0') {
            std::abort();
        }

        poses.emplace_back(x, y);
    }
    if (!f.eof()) {
        std::abort();
    }

    std::int64_t p1 = 0;
    for (std::size_t i = 0; i + 1 < poses.size(); ++i) {
        for (std::size_t j = i + 1; j < poses.size(); ++j) {
            const auto &a = poses[i];
            const auto &b = poses[j];
            const auto w = std::abs(a.first - b.first) + 1;
            const auto h = std::abs(a.second - b.second) + 1;
            p1 = std::max(p1, w * h);
        }
    }

    std::println("Day9: P1: {}", p1);
}
