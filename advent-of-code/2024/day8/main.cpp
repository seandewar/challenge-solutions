#include <fstream>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

int main()
{
    std::ifstream file{"input"};
    if (!file)
        throw std::runtime_error{"failed to open input file"};

    std::unordered_map<char, std::vector<std::pair<int, int>>> antennas;
    int w = 0, h = 0;
    for (std::string line; std::getline(file, line); ++h) {
        for (std::size_t x = 0;; ++x) {
            x = line.find_first_not_of(".", x);
            if (x == std::string::npos)
                break;
            antennas[line[x]].emplace_back(x, h);
        }
        w = std::max<int>(w, line.size());
    }

    if (!file.eof())
        throw std::runtime_error{"failed to read input file"};

    std::unordered_set<std::size_t> p1_antinode_is, p2_antinode_is;
    for (const auto &[c, as] : antennas) {
        for (const auto [x1, y1] : as) {
            for (const auto [x2, y2] : as) {
                if (x1 == x2 && y1 == y2)
                    continue;

                const int dx = x1 - x2, dy = y1 - y2;
                if (const int anti_x = x1 - 2 * dx, anti_y = y1 - 2 * dy;
                    anti_x < 0 || anti_y < 0 || anti_x >= w || anti_y >= h)
                    p1_antinode_is.insert(anti_y * w + anti_x);

                for (int anti_x = x1 - dx, anti_y = y1 - dy;
                     anti_x >= 0 && anti_y >= 0 && anti_x < w && anti_y < h;
                     anti_x -= dx, anti_y -= dy)
                    p2_antinode_is.insert(anti_y * w + anti_x);
            }
        }
    }

    std::cout << "Day8: P1: " << p1_antinode_is.size()
              << ", P2: " << p2_antinode_is.size() << std::endl;
}
