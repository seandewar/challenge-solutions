#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct Grid {
    std::string squares;
    int width = 0, height = 0;
    int start_rx = -1, start_ry = -1;

    bool in_bounds(const int x, const int y) const
    {
        return x >= 0 && y >= 0 && x < width && y < height;
    }

    char &square(const int x, const int y)
    {
        return squares[y * width + x];
    }
};

static unsigned simulate(Grid &grid, const std::string_view instrs)
{
    std::vector<std::pair<int, int>> wide_boxes_to_move;
    int rx = grid.start_rx, ry = grid.start_ry;

    for (const char c : instrs) {
        int dx = 0, dy = 0;
        switch (c) {
        case '>':
            dx = 1;
            break;
        case '<':
            dx = -1;
            break;
        case '^':
            dy = -1;
            break;
        case 'v':
            dy = 1;
            break;
        default:
            throw std::runtime_error{"malformed input (unknown instruction)"};
        }

        const int rx2 = rx + dx, ry2 = ry + dy;
        if (!grid.in_bounds(rx2, ry2))
            continue;

        const auto vert_moved_wide_boxes = [&](this auto const &self,
                                               const int x, const int y) {
            if (!grid.in_bounds(x, y))
                return false;

            switch (const char c = grid.square(x, y)) {
            case '#':
                return false;
            case '.':
                return true;

            case '[':
            case ']': {
                const int x2 = c == '[' ? x + 1 : x - 1;
                if (!self(x, y + dy) || !self(x2, y + dy))
                    return false;

                wide_boxes_to_move.emplace_back(x, y);
                wide_boxes_to_move.emplace_back(x2, y);
                return true;
            }

            default:
                throw std::runtime_error{"malformed input (unknown square)"};
            }
        };

        switch (const char c = grid.square(rx2, ry2)) {
        case 'O':
        case '[':
        case ']':
            if (c == 'O' || dy == 0) {
                int bx = rx2, by = ry2;
                for (; grid.in_bounds(bx, by); bx += dx, by += dy) {
                    const char c = grid.square(bx, by);
                    if (c != 'O' && c != '[' && c != ']')
                        break;
                }
                if (!grid.in_bounds(bx, by) || grid.square(bx, by) != '.')
                    break;

                for (; bx != rx2 || by != ry2; bx -= dx, by -= dy)
                    grid.square(bx, by) = grid.square(bx - dx, by - dy);
                grid.square(rx2, ry2) = '.';
            } else {
                wide_boxes_to_move.clear();
                if (!vert_moved_wide_boxes(rx2, ry2))
                    break;

                std::sort(wide_boxes_to_move.begin(), wide_boxes_to_move.end(),
                          [dy](const auto &a, const auto &b) {
                              return (dy < 0 ? a.second < b.second
                                             : a.second > b.second)
                                     || (a.second == b.second
                                         && a.first < b.first);
                          });
                wide_boxes_to_move.erase(std::unique(wide_boxes_to_move.begin(),
                                                     wide_boxes_to_move.end()),
                                         wide_boxes_to_move.end());

                for (const auto [x, y] : wide_boxes_to_move) {
                    grid.square(x, y + dy) = grid.square(x, y);
                    grid.square(x, y) = '.';
                }
            }

        case '.':
            rx = rx2;
            ry = ry2;
        case '#':
            break;
        default:
            throw std::runtime_error{"malformed input (unknown square)"};
        }
    }

    unsigned ret = 0;
    for (int y = 0; y < grid.height; ++y) {
        for (int x = 0; x < grid.width; ++x) {
            const char c = grid.square(x, y);
            if (c == 'O' || c == '[')
                ret += 100 * y + x;
        }
    }
    return ret;
}

int main()
{
    std::ifstream file{"input"};
    if (!file)
        throw std::runtime_error{"failed to open input file"};

    std::string line;
    Grid p1_grid;
    for (; std::getline(file, line); p1_grid.height++) {
        if (line.empty())
            break;

        if (const auto i = line.find_first_of('@'); i != std::string::npos) {
            if (p1_grid.start_rx >= 0
                || line.find_first_of('@', i + 1) != std::string::npos)
                throw std::runtime_error{"malformed input (many robots)"};
            p1_grid.start_rx = i;
            p1_grid.start_ry = p1_grid.height;
            line[i] = '.';
        }

        std::copy(line.begin(), line.end(), back_inserter(p1_grid.squares));
        if (p1_grid.width == 0)
            p1_grid.width = line.size();
        else if (p1_grid.width != line.size())
            throw std::runtime_error{"malformed input (width differs)"};
    }
    if (p1_grid.start_rx < 0)
        throw std::runtime_error{"malformed input (no robot)"};

    std::string instrs;
    while (std::getline(file, line))
        std::copy(line.begin(), line.end(), back_inserter(instrs));

    if (!file.eof())
        throw std::runtime_error{"failed to read input file"};

    Grid p2_grid{.width = p1_grid.width * 2,
                 .height = p1_grid.height,
                 .start_rx = p1_grid.start_rx * 2,
                 .start_ry = p1_grid.start_ry};
    p2_grid.squares.reserve(p2_grid.width * p2_grid.height);
    for (const char c : p1_grid.squares) {
        switch (c) {
        case '.':
            p2_grid.squares.push_back('.');
            p2_grid.squares.push_back('.');
            break;
        case 'O':
            p2_grid.squares.push_back('[');
            p2_grid.squares.push_back(']');
            break;
        case '#':
            p2_grid.squares.push_back('#');
            p2_grid.squares.push_back('#');
            break;
        default:
            throw std::runtime_error{"malformed input (unknown square)"};
        }
    }

    std::cout << "Day15: P1: " << simulate(p1_grid, instrs) << std::flush;
    std::cout << ", P2: " << simulate(p2_grid, instrs) << std::endl;
}
