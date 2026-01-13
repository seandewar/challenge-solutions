#include <algorithm>
#include <fstream>
#include <print>
#include <string>

int main()
{
    std::ifstream f("input");
    if (!f) {
        std::abort();
    }

    int width = 0;
    int height = 0;
    std::string grid;
    std::string line;
    while (std::getline(f, line)) {
        if (!grid.empty() && line.size() != static_cast<std::size_t>(width)) {
            std::abort();
        }
        grid += line;
        width = line.size();
        ++height;
    }

    unsigned total_removed = 0;

    bool p1 = true;
    while (true) {
        bool did_remove = false;
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                char &c = grid[y * width + x];
                if (c != '@') {
                    continue;
                }

                unsigned adjacent = 0;
                for (const auto &[dx, dy] : {std::pair{-1, -1},
                                             {0, -1},
                                             {1, -1},
                                             {-1, 0},
                                             {1, 0},
                                             {-1, 1},
                                             {0, 1},
                                             {1, 1}}) {
                    const int x2 = x + dx;
                    const int y2 = y + dy;
                    if (x2 < 0 || x2 >= width || y2 < 0 || y2 >= height) {
                        continue;
                    }
                    const char c2 = grid[y2 * width + x2];
                    if (c2 != '@' && c2 != 'X') {
                        continue;
                    }
                    if (++adjacent >= 4) {
                        break;
                    }
                }

                if (adjacent < 4) {
                    ++total_removed;
                    did_remove = true;
                    c = 'X'; // mark as removed
                }
            }
        }

        if (p1) {
            std::print("Day4: P1: {}", total_removed);
            p1 = false;
        }
        if (!did_remove) {
            break;
        }
        // Actually remove marked rolls. We could avoid a sweep by setting them
        // to the iteration number instead, but I'm not optimizing this.
        // (Plus, we could avoid traversing the whole grid every iteration by
        // using a queue...)
        std::ranges::replace(grid, 'X', '.');
    }

    if (p1) {
        std::abort();
    }
    std::println(", P2: {}", total_removed);
}
