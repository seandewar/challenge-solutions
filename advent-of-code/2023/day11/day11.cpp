// Took me a while to realize that you don't need Dijkstra... or BFS... the
// Manhattan distance is enough!
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <utility>
#include <vector>

int main() {
    std::ifstream f{"input"};
    assert(f);

    std::vector<char> grid;
    int w = 0, h = 0;
    std::set<int> empty_rows, empty_cols;
    std::vector<std::pair<int, int>> galaxies;
    for (std::string line; std::getline(f, line); ++h) {
        if (w == 0)
            w = static_cast<int>(line.size());
        assert(static_cast<size_t>(w) == line.size());

        const auto old_galaxies_count = galaxies.size();
        for (size_t x = 0; (x = line.find('#', x)) != std::string::npos; ++x)
            galaxies.emplace_back(static_cast<int>(x), h);
        if (galaxies.size() == old_galaxies_count)
            empty_rows.insert(h);
        std::ranges::copy(line, std::back_inserter(grid));
    }
    assert(f.eof() && !galaxies.empty());
    const auto gi = [=](const int x, const int y) { return w * y + x; };
    for (int x = 0; x < w; ++x) {
        int y = 0;
        for (; y < h && grid[gi(x, y)] == '.'; ++y) {}
        if (y == h)
            empty_cols.insert(x);
    }

    std::int_least64_t p1 = 0, p2 = 0;
    for (size_t i = 0; i < galaxies.size(); ++i) {
        const auto [ax, ay] = galaxies[i];
        for (size_t j = i + 1; j < galaxies.size(); ++j) {
            const auto [bx, by] = galaxies[j];
            const auto [min_x, max_x] = std::minmax(ax, bx);
            const auto [min_y, max_y] = std::minmax(ay, by);
            const std::int_least64_t d = max_x - min_x + max_y - min_y;

            const auto big_count =
                std::distance(empty_rows.lower_bound(min_y),
                              empty_rows.upper_bound(max_y))
                + std::distance(empty_cols.lower_bound(min_x),
                                empty_cols.upper_bound(max_x));
            p1 += d + big_count;
            p2 += d + big_count * 999'999;
        }
    }
    std::cout << "Day11: P1: " << p1 << ", P2: " << p2 << '\n';
}
