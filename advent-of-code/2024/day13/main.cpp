#include <algorithm>
#include <charconv>
#include <cstdint>
#include <fstream>
#include <iostream>

int main()
{
    std::ifstream file{"input"};
    if (!file)
        throw std::runtime_error{"failed to open input file"};

    std::int_least64_t p1 = 0, p2 = 0;
    for (std::string line; std::getline(file, line);) {
        if (std::all_of(line.begin(), line.end(),
                        [](const char c) { return std::isspace(c); }))
            continue;

        const auto parse_line = [&line](const bool is_buttons) {
            const size_t x_i = line.find(is_buttons ? "X+" : "X="),
                         y_i = line.find(is_buttons ? "Y+" : "Y=", x_i);
            if (x_i == std::string::npos || y_i == std::string::npos)
                throw std::runtime_error{"malformed input"};

            std::int_least64_t x, y;
            const auto [after_x, x_ec] =
                std::from_chars(line.data() + x_i + 2, line.data() + y_i, x);
            const auto [after_y, y_ec] = std::from_chars(
                line.data() + y_i + 2, line.data() + line.size(), y);
            if (x_ec != std::errc{} || y_ec != std::errc{})
                throw std::runtime_error{"malformed input"};

            return std::pair{x, y};
        };
        const auto [ax, ay] = parse_line(true);

        if (!std::getline(file, line))
            break;
        const auto [bx, by] = parse_line(true);

        if (!std::getline(file, line))
            break;
        const auto [tx, ty] = parse_line(false);

        const auto solve =
            [=](const std::int_least64_t tx,
                const std::int_least64_t ty) -> std::int_least64_t {
            const auto b_numer = ax * ty - ay * tx, b_denom = ax * by - ay * bx;
            if (b_numer % b_denom != 0)
                return 0;

            const auto b = b_numer / b_denom, a_numer = tx - b * bx,
                       a_denom = ax;
            if (b < 0 || a_numer % a_denom != 0)
                return 0;

            const auto a = a_numer / a_denom;
            if (a < 0)
                return 0;

            return 3 * a + b;
        };

        p1 += solve(tx, ty);
        p2 += solve(tx + 10000000000000, ty + 10000000000000);
    }
    if (!file.eof())
        throw std::runtime_error{"failed to read input file"};

    std::cout << "Day12: P1: " << p1 << ", P2: " << p2 << std::endl;
}
