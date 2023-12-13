#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <utility>

int main() {
    std::ifstream f{"input"};
    assert(f.is_open());
    int w = 0, h = 0;
    std::string grid;
    const auto gi = [&](const int x, const int y) { return y * w + x; };
    const auto compute_answer = [&](const bool p1, const int skip_x = -1,
                                    const int skip_y = -1) {
        for (int y = 0; y < h - 1; ++y) {
            if (y == skip_y)
                continue;
            bool unsmudged = p1;
            for (int y1 = y, y2 = y + 1; y1 >= 0 && y2 < h; --y1, ++y2) {
                const auto y1_end = grid.cbegin() + gi(0, y1 + 1),
                           y2_begin = grid.cbegin() + gi(0, y2);
                const auto [y1_mismatch, y2_mismatch] =
                    std::mismatch(grid.cbegin() + gi(0, y1), y1_end, y2_begin);
                if (y1_mismatch == y1_end)
                    continue;
                if (std::exchange(unsmudged, true)
                    || !std::equal(y1_mismatch + 1, y1_end, y2_mismatch + 1))
                    goto no_y_match;
            }
            return std::make_tuple((y + 1) * 100, -1, y);
        no_y_match:;
        }

        for (int x = 0; x < w - 1; ++x) {
            if (x == skip_x)
                continue;
            bool unsmudged = p1;
            for (int x1 = x, x2 = x + 1; x1 >= 0 && x2 < w; --x1, ++x2) {
                for (int y = 0; y < h; ++y) {
                    if (grid[gi(x1, y)] != grid[gi(x2, y)]
                        && std::exchange(unsmudged, true))
                        goto no_x_match;
                }
            }
            return std::make_tuple(x + 1, x, -1);
        no_x_match:;
        }
        std::abort();
    };

    int p1 = 0, p2 = 0;
    for (std::string line; std::getline(f, line) || h > 0;) {
        if (line.empty()) {
            const auto [p1_result, p1_x, p1_y] = compute_answer(true);
            p1 += p1_result;
            p2 += std::get<0>(compute_answer(false, p1_x, p1_y));
            h = w = 0;
            grid.clear();
            continue;
        }
        if (w == 0)
            w = line.size();
        assert(line.size() == static_cast<std::size_t>(w));
        ++h;
        std::copy(line.cbegin(), line.cend(), std::back_inserter(grid));
    }
    assert(f.eof());
    std::cout << "Day13: P1: " << p1 << ", P2: " << p2 << '\n';
}
