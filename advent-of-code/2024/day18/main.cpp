#include <algorithm>
#include <array>
#include <charconv>
#include <climits>
#include <deque>
#include <fstream>
#include <iostream>
#include <limits>
#include <optional>
#include <unordered_set>
#include <utility>

int main()
{
    std::ifstream file{"input"};
    if (!file)
        throw std::runtime_error{"failed to open input file"};

    constexpr unsigned dim = 71, p1_byte_count = 1024;

    constexpr auto byte_square = std::numeric_limits<unsigned>::max(),
                   unvisited_square = byte_square - 1;
    std::array<unsigned, dim * dim> grid;
    grid.fill(unvisited_square);
    const auto square = [&](const unsigned x, const unsigned y) -> unsigned & {
        return grid[y * dim + x];
    };

    std::string line;
    const auto parse_next_byte_pos =
        [&]() -> std::optional<std::pair<unsigned, unsigned>> {
        if (!std::getline(file, line))
            return {};

        unsigned x, y;
        const auto [after_x, x_ec] =
            std::from_chars(line.data(), line.data() + line.size(), x);
        if (x_ec != std::errc{} || *after_x == '\0')
            throw std::runtime_error{"malformed input"};

        const auto [after_y, y_ec] =
            std::from_chars(after_x + 1, line.data() + line.size(), y);
        if (y_ec != std::errc{})
            throw std::runtime_error{"malformed input"};

        return {{x, y}};
    };
    for (unsigned i = 0; i < p1_byte_count; ++i) {
        const auto pos = parse_next_byte_pos();
        if (!pos)
            break;
        square(pos->first, pos->second) = byte_square;
    }

    const auto pos_hash = [](const std::pair<unsigned, unsigned> &p) {
        return static_cast<std::size_t>(p.first)
                   << ((sizeof p.first) * CHAR_BIT)
               | p.second;
    };
    std::unordered_set<std::pair<unsigned, unsigned>, decltype(pos_hash)>
        best_path;

    struct Walk {
        std::pair<int, int> pos;
        std::unordered_set<std::pair<unsigned, unsigned>, decltype(pos_hash)>
            path;
    };
    std::deque<Walk> q;

    const auto find_path = [&]() {
        std::transform(grid.begin(), grid.end(), grid.begin(),
                       [&](const auto s) {
                           return s != byte_square ? unvisited_square : s;
                       });
        square(0, 0) = 0;
        q.clear();
        q.emplace_back(std::pair{0, 0});

        for (unsigned steps = 0; !q.empty(); ++steps) {
            const auto size = q.size();
            for (std::size_t i = 0; i < size; ++i) {
                const auto [x, y] = q.front().pos;
                auto path = std::move(q.front().path);
                q.pop_front();

                path.emplace(x, y);
                if (x == dim - 1 && y == dim - 1) {
                    best_path = std::move(path);
                    return true;
                }

                for (const auto [dx, dy] :
                     {std::pair{1, 0}, {0, 1}, {-1, 0}, {0, -1}}) {
                    const int x2 = x + dx, y2 = y + dy;
                    if (x2 < 0 || y2 < 0 || x2 >= dim || y2 >= dim)
                        continue;
                    if (square(x2, y2) != unvisited_square)
                        continue;

                    square(x2, y2) = steps + 1;
                    q.emplace_back(std::pair{x2, y2}, path);
                }
            }
        }

        return false;
    };

    if (!find_path())
        throw std::runtime_error{"end unreachable"};
    std::cout << "Day18: P1: " << best_path.size() - 1 << std::flush;

    for (std::optional<std::pair<unsigned, unsigned>> pos;
         (pos = parse_next_byte_pos());) {
        const auto [x, y] = *pos;
        square(x, y) = byte_square;
        if (!best_path.contains({x, y}))
            continue;

        if (!find_path()) {
            std::cout << ", P2: " << x << ',' << y << std::endl;
            return EXIT_SUCCESS;
        }
    }
    if (!file.eof())
        throw std::runtime_error{"failed to read input"};

    throw std::runtime_error{"P2 unsolvable; end always reachable"};
}
