#include <algorithm>
#include <array>
#include <charconv>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

int main()
{
    std::fstream file{"input"};
    if (!file)
        throw std::runtime_error{"failed to open input file"};

    const auto mod = [](const auto a, const auto b) {
        const auto modu = a % b;
        return modu + (modu < 0) * b;
    };
    struct Robot {
        int x, y;
        int vx, vy;
    };

    const int width = 101, height = 103;
    std::array<unsigned, 4> p1_quads{};
    std::vector<Robot> p2_robots;
    for (std::string line; std::getline(file, line);) {
        const std::size_t pi = line.find_first_of('='),
                          vi = line.find_first_of('=', pi + 1);
        if (pi == std::string::npos || vi == std::string::npos)
            throw std::runtime_error{"malformed input"};

        Robot robot;
        const auto [after_px, ec_px] =
            std::from_chars(line.data() + pi + 1, line.data() + vi, robot.x);
        const auto [after_py, ec_py] =
            std::from_chars(after_px + 1, line.data() + vi, robot.y);
        const auto [after_vx, ec_vx] = std::from_chars(
            line.data() + vi + 1, line.data() + line.size(), robot.vx);
        const auto [after_vy, ec_vy] =
            std::from_chars(after_vx + 1, line.data() + line.size(), robot.vy);

        if (ec_px != std::errc{} || ec_py != std::errc{} || ec_vx != std::errc{}
            || ec_vy != std::errc{})
            throw std::runtime_error{"malformed input"};

        p2_robots.push_back(robot);

        const int p1x = mod(robot.x + 100 * robot.vx, width),
                  p1y = mod(robot.y + 100 * robot.vy, height);
        if (p1x != width / 2 && p1y != height / 2)
            ++p1_quads[2 * (p1y > height / 2) + (p1x > width / 2)];
    }

    if (!file.eof())
        throw std::runtime_error{"failed to read input file"};

    const auto p1 =
        std::accumulate(p1_quads.begin(), p1_quads.end(), 1, std::multiplies{});
    std::cout << "Day14: P1: " << p1 << std::endl;

    std::vector<std::size_t> robot_is_by_pos(p2_robots.size());
    std::iota(robot_is_by_pos.begin(), robot_is_by_pos.end(), 0);

    for (unsigned t = 0;; ++t) {
        if (t % 500 == 0)
            std::cout << "P2: t=" << t << " ...\n";

        std::sort(robot_is_by_pos.begin(), robot_is_by_pos.end(),
                  [&](const std::size_t i, const std::size_t j) {
                      const auto &a = p2_robots[i], &b = p2_robots[j];
                      return std::tie(a.y, a.x) < std::tie(b.y, b.x);
                  });

        // P2 christmas tree has many lines; try looking for a horizontal one.
        for (size_t i = 0; i < robot_is_by_pos.size();) {
            size_t j = i + 1;
            for (; j < robot_is_by_pos.size(); ++j) {
                const auto &a = p2_robots[robot_is_by_pos[j - 1]],
                           &b = p2_robots[robot_is_by_pos[j]];
                if (a.x + 1 != b.x || a.y != b.y)
                    break;

                if (j - i >= 20) { // A width of 20 should be enough...
                    std::size_t i = 0;
                    for (unsigned y = 0; y < height; ++y) {
                        for (unsigned x = 0; x < width; ++x) {
                            unsigned rn = 0;
                            for (; i < robot_is_by_pos.size() && rn < 10;
                                 ++rn, ++i) {
                                const auto &r = p2_robots[robot_is_by_pos[i]];
                                if (std::tie(r.x, r.y) != std::tie(x, y))
                                    break;
                            }
                            if (rn > 9)
                                std::cout << '#';
                            else if (rn > 0)
                                std::cout << rn;
                            else
                                std::cout << '.';
                        }
                        std::cout << '\n';
                    }

                    std::cout << "Day14: P1: " << p1 << ", P2: " << t
                              << std::endl;
                    return EXIT_SUCCESS;
                }
            }
            i = j;
        }

        for (auto &r : p2_robots) {
            r.x = mod(r.x + r.vx, width);
            r.y = mod(r.y + r.vy, height);
        }
    }
}
