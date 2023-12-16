#include <algorithm>
#include <cassert>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

struct Cell {
    char c;
    std::uint8_t visited_dirs = 0;
};

enum Dir : std::uint8_t {
    UP = 0x1,
    DOWN = 0x2,
    LEFT = 0x4,
    RIGHT = 0x8,
};

int main() {
    std::ifstream f{"input"};
    assert(f.is_open());
    std::vector<Cell> grid;
    int w = 0, h = 0;
    for (std::string line; std::getline(f, line); ++h) {
        if (w == 0)
            w = line.size();
        assert(line.size() == static_cast<std::size_t>(w));
        std::ranges::transform(line, std::back_inserter(grid),
                               [](const char c) { return Cell{c}; });
    }
    assert(f.eof() && w > 0 && h > 0);

    std::vector<std::tuple<int, int, Dir>> beams;
    const auto gi = [=](const int x, const int y) { return y * w + x; };
    const auto beamify = [&](const int start_x, const int start_y,
                             const Dir start_dir) {
        for (auto &cell : grid)
            cell.visited_dirs = 0;
        grid[gi(start_x, start_y)].visited_dirs |= start_dir;
        beams.clear();
        beams.emplace_back(start_x, start_y, start_dir);

        int energized = 1;
        while (!beams.empty()) {
            auto &[x, y, dir] = beams.back();
            x += dir == LEFT ? -1 : (dir == RIGHT ? 1 : 0);
            y += dir == UP ? -1 : (dir == DOWN ? 1 : 0);
            if (x < 0 || x >= w || y < 0 || y >= h) {
                beams.pop_back();
                continue;
            }
            auto &cell = grid[gi(x, y)];
            if (cell.visited_dirs & dir) {
                beams.pop_back();
                continue;
            }
            if (cell.visited_dirs == 0)
                ++energized;
            cell.visited_dirs |= dir;

            if (cell.c == '/') {
                dir = (dir == UP
                           ? RIGHT
                           : (dir == DOWN ? LEFT : (dir == LEFT ? DOWN : UP)));
            } else if (cell.c == '\\') {
                dir = (dir == UP
                           ? LEFT
                           : (dir == DOWN ? RIGHT : (dir == LEFT ? UP : DOWN)));
            } else if (cell.c == '-' && (dir == UP || dir == DOWN)) {
                dir = LEFT;
                beams.emplace_back(x, y, RIGHT);
            } else if (cell.c == '|' && (dir == LEFT || dir == RIGHT)) {
                dir = UP;
                beams.emplace_back(x, y, DOWN);
            }
        }
        return energized;
    };

    int max_energized = beamify(0, 0, RIGHT);
    std::cout << "Day16: P1: " << max_energized << std::flush;
    max_energized = std::max(max_energized, beamify(w - 1, 0, LEFT));
    for (int y = 1; y < h; ++y) {
        max_energized = std::max(max_energized, beamify(0, y, RIGHT));
        max_energized = std::max(max_energized, beamify(w - 1, y, LEFT));
    }
    for (int x = 0; x < w; ++x) {
        max_energized = std::max(max_energized, beamify(x, 0, DOWN));
        max_energized = std::max(max_energized, beamify(x, h - 1, UP));
    }
    std::cout << ", P2: " << max_energized << '\n';
}
