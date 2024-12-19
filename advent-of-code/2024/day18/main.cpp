#include <algorithm>
#include <array>
#include <charconv>
#include <climits>
#include <deque>
#include <fstream>
#include <iostream>
#include <limits>
#include <unordered_set>
#include <utility>
#include <vector>

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

    std::vector<std::pair<unsigned, unsigned>> bytes;
    for (std::string line; std::getline(file, line);) {
        unsigned x, y;
        const auto [after_x, x_ec] =
            std::from_chars(line.data(), line.data() + line.size(), x);
        if (x_ec != std::errc{} || *after_x == '\0')
            throw std::runtime_error{"malformed input"};

        const auto [after_y, y_ec] =
            std::from_chars(after_x + 1, line.data() + line.size(), y);
        if (y_ec != std::errc{})
            throw std::runtime_error{"malformed input"};

        bytes.emplace_back(x, y);
    };
    if (!file.eof())
        throw std::runtime_error{"failed to read input"};

    unsigned next_byte_i = 0;
    for (; next_byte_i < p1_byte_count; ++next_byte_i) {
        if (std::exchange(
                square(bytes[next_byte_i].first, bytes[next_byte_i].second),
                byte_square)
            == byte_square)
            throw std::runtime_error{"duplicate byte positions unsupported"};
    }

    const auto pos_hash = [](const std::pair<unsigned, unsigned> &p) {
        return static_cast<std::size_t>(p.first)
                   << ((sizeof p.first) * CHAR_BIT)
               | p.second;
    };
    std::unordered_set<std::pair<unsigned, unsigned>, decltype(pos_hash)>
        goal_path_set;

    struct Walk {
        std::pair<int, int> pos;
        std::unordered_set<std::pair<unsigned, unsigned>, decltype(pos_hash)>
            path;
    };
    std::deque<Walk> q;

    const auto find_best_path = [&] {
        goal_path_set.clear();
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
                    goal_path_set = std::move(path);
                    return;
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
    };

    find_best_path();
    if (goal_path_set.empty())
        throw std::runtime_error{"end unreachable"};
    std::cout << "Day18: P1: " << goal_path_set.size() - 1 << std::flush;

    const auto p2_it =
        std::partition_point(bytes.begin(), bytes.end(), [&](const auto &pos) {
            const unsigned i = &pos - bytes.data();
            bool path_stale = false;
            if (i <= next_byte_i) {
                while (next_byte_i > i + 1) {
                    --next_byte_i;
                    square(bytes[next_byte_i].first,
                           bytes[next_byte_i].second) = unvisited_square;
                }
                path_stale = goal_path_set.empty();
            } else {
                for (; next_byte_i <= i; ++next_byte_i) {
                    path_stale = path_stale
                                 || goal_path_set.contains(bytes[next_byte_i]);
                    if (std::exchange(square(bytes[next_byte_i].first,
                                             bytes[next_byte_i].second),
                                      byte_square)
                        == byte_square)
                        throw std::runtime_error{
                            "duplicate byte positions unsupported"};
                }
            }

            if (path_stale)
                find_best_path();
            return !goal_path_set.empty();
        });
    if (p2_it == bytes.end())
        throw std::runtime_error{"P2 unsolvable; end always reachable"};

    std::cout << ", P2: " << p2_it->first << ',' << p2_it->second << std::endl;
}
