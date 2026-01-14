#include <algorithm>
#include <cstdint>
#include <fstream>
#include <limits>
#include <numeric>
#include <print>
#include <string>
#include <vector>

int main()
{
    std::ifstream f("input");
    if (!f.is_open()) {
        std::abort();
    }

    constexpr auto mirror = std::numeric_limits<std::uint32_t>::max();
    // We could probably get by with just 2 rows, but idc.
    std::vector<std::size_t> grid;
    std::size_t width = 0;
    std::size_t height = 0;

    std::string line;
    while (std::getline(f, line)) {
        if (width > 0 && line.size() != width) {
            std::abort();
        }
        std::ranges::transform(line, std::back_inserter(grid),
                               [](const char c) {
                                   return c == '^' ? mirror : c == 'S' ? 1 : 0;
                               });
        width = line.size();
        ++height;
    }

    const auto idx = [&width](const std::size_t x, const std::size_t y) {
        return y * width + x;
    };

    std::size_t p1 = 0;
    for (std::size_t y = 0; y < height; ++y) {
        for (std::size_t x = 0; x < width; ++x) {
            auto &c = grid[idx(x, y)];
            if (c == 0 || c == mirror) {
                continue;
            }
            if (y + 1 >= height) {
                continue;
            }
            auto &c2 = grid[idx(x, y + 1)];
            if (c2 == mirror) {
                if (x > 0) {
                    auto &c3 = grid[idx(x - 1, y + 1)];
                    if (c3 == mirror) {
                        std::abort();
                    }
                    c3 += c;
                }
                if (x + 1 < width) {
                    auto &c3 = grid[idx(x + 1, y + 1)];
                    if (c3 == mirror) {
                        std::abort();
                    }
                    c3 += c;
                }
                ++p1;
            } else {
                c2 += c;
            }
        }
    }

    std::println("Day7: P1: {}, P2: {}", p1,
                 std::accumulate(grid.begin() + idx(0, height - 1), grid.end(),
                                 0z, [](auto acc, const auto c) {
                                     if (c != mirror) {
                                         acc += c;
                                     }
                                     return acc;
                                 }));
}
