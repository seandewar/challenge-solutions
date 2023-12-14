#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

int main() {
    std::ifstream f{"input"};
    assert(f.is_open());
    std::string grid;
    int w = 0, h = 0;
    for (std::string line; std::getline(f, line); ++h) {
        if (w == 0)
            w = line.size();
        assert(line.size() == static_cast<std::size_t>(w));
        grid += line;
    }
    assert(f.eof());

    const auto gi = [=](const int x, const int y) { return y * w + x; };
    const auto tilt_cell = [&](const int x, const int y, const bool horiz,
                               const int delta, int &dst) {
        char &c = grid[gi(x, y)];
        if (c != 'O') {
            if (c == '#')
                dst = (horiz ? x : y) + delta;
            return false;
        }
        c = '.';
        grid[horiz ? gi(dst, y) : gi(x, dst)] = 'O';
        dst += delta;
        return true;
    };

    std::unordered_map<std::string, int> grids;
    for (int i = 0;; ++i) {
        for (int x = 0; x < w; ++x)
            for (int y = 0, y_dst = 0; y < h; ++y)
                tilt_cell(x, y, false, 1, y_dst);
        for (int y = 0; y < h; ++y)
            for (int x = 0, x_dst = 0; x < w; ++x)
                tilt_cell(x, y, true, 1, x_dst);

        int load_after_i = 0, load_after_n_or_w = 0;
        for (int x = 0; x < w; ++x) {
            for (int y = h - 1, y_dst = h - 1; y >= 0; --y) {
                if (tilt_cell(x, y, false, -1, y_dst)) {
                    load_after_n_or_w += h - y;
                    load_after_i += h - y_dst - 1;
                }
            }
        }

        if (i == 0)
            std::cout << "Day14: P1: " << load_after_n_or_w << std::flush;

        if (const auto [it, new_grid] = grids.emplace(grid, i); !new_grid) {
            const auto i_diff = i - it->second, i_remaining = 999'999'999 - i;
            i += i_remaining - (i_remaining % i_diff);
        }
        if (i == 999'999'999) {
            std::cout << ", P2: " << load_after_i << '\n';
            break;
        }

        for (int y = 0; y < h; ++y)
            for (int x = w - 1, x_dst = w - 1; x >= 0; --x)
                tilt_cell(x, y, true, -1, x_dst);
    }
}
