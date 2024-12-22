#include <array>
#include <cassert>
#include <charconv>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <unordered_map>

int main()
{
    std::ifstream file{"input"};
    if (!file)
        std::abort();

    using Pos = std::pair<int, int>;

    constexpr Pos numpad_gap{0, 3};
    const auto numpad_pos = [](const char c) -> Pos {
        switch (c) {
        case '7':
            return {0, 0};
        case '8':
            return {1, 0};
        case '9':
            return {2, 0};
        case '4':
            return {0, 1};
        case '5':
            return {1, 1};
        case '6':
            return {2, 1};
        case '1':
            return {0, 2};
        case '2':
            return {1, 2};
        case '3':
            return {2, 2};
        case '0':
            return {1, 3};
        case 'A':
            return {2, 3};

        default:
            std::abort();
        }
    };

    constexpr Pos dirpad_gap{0, 0};
    const auto dirpad_pos = [](const char c) -> Pos {
        switch (c) {
        case '^':
            return {1, 0};
        case 'A':
            return {2, 0};
        case '<':
            return {0, 1};
        case 'v':
            return {1, 1};
        case '>':
            return {2, 1};

        default:
            std::abort();
        }
    };

    constexpr std::size_t p1_num_dirpads = 3, p2_num_dirpads = 26;
    std::array<std::string, p2_num_dirpads> move_bufs;
    std::unordered_map<std::string, std::pair<std::size_t, std::size_t>> memo;
    std::ostringstream memo_key_ss;

    std::size_t p1 = 0, p2 = 0;
    for (std::string line; std::getline(file, line);) {
        unsigned num_part;
        const auto [after_num_part, ec] =
            std::from_chars(line.data(), line.data() + line.size(), num_part);

        if (ec != std::errc{} || *after_num_part != 'A'
            || static_cast<std::size_t>(after_num_part - line.data()) + 1
                   != line.size())
            std::abort();

        // Numeric keypad is pad_i == 0; directional keypads are the others.
        const auto sum_min_moves = [&](this const auto &self,
                                       const std::size_t pad_i,
                                       std::size_t &out_p1_min_moves,
                                       std::size_t &out_p2_min_moves) {
            const auto &keys = pad_i == 0 ? line : move_bufs[pad_i - 1];

            memo_key_ss.clear();
            memo_key_ss.str({});
            memo_key_ss << pad_i;
            memo_key_ss << '|';
            memo_key_ss << keys;
            const auto memo_key = memo_key_ss.str();

            if (const auto it = memo.find(memo_key); it != memo.end()) {
                const auto [p1_min_moves, p2_min_moves] = it->second;
                out_p1_min_moves = std::min(out_p1_min_moves, p1_min_moves);
                out_p2_min_moves = std::min(out_p2_min_moves, p2_min_moves);
                return;
            }

            if (pad_i == p1_num_dirpads)
                out_p1_min_moves = std::min(out_p1_min_moves, keys.size());
            if (pad_i == p2_num_dirpads) {
                out_p2_min_moves = std::min(out_p2_min_moves, keys.size());
                return;
            }

            auto &move_buf = move_bufs[pad_i];
            const auto &gap = pad_i == 0 ? numpad_gap : dirpad_gap;

            std::size_t p1_total_min_moves = 0, p2_total_min_moves = 0;

            for (auto pos = pad_i == 0 ? numpad_pos('A') : dirpad_pos('A');
                 const char c : keys) {
                auto &[x, y] = pos;

                const auto [x2, y2] =
                    pad_i == 0 ? numpad_pos(c) : dirpad_pos(c);
                const auto dx = x2 - x, dy = y2 - y;
                const std::size_t move_len = std::abs(dx) + std::abs(dy) + 1;

                auto p1_min_moves = std::numeric_limits<std::size_t>::max(),
                     p2_min_moves = std::numeric_limits<std::size_t>::max();

                const bool move_horiz_first = std::pair{x2, y} != gap;
                if (move_horiz_first) {
                    std::fill_n(back_inserter(move_buf), std::abs(dx),
                                dx > 0 ? '>' : '<');
                    std::fill_n(back_inserter(move_buf), std::abs(dy),
                                dy > 0 ? 'v' : '^');
                    move_buf.push_back('A');

                    self(pad_i + 1, p1_min_moves, p2_min_moves);
                    move_buf.resize(move_buf.size() - move_len);
                }

                // No need to take this branch if it results in the same moves,
                // or if we're processing the final robot and already branched.
                if (std::pair{x, y2} != gap && dx != 0 && dy != 0
                    && (pad_i < p2_num_dirpads - 1 || !move_horiz_first)) {
                    std::fill_n(back_inserter(move_buf), std::abs(dy),
                                dy > 0 ? 'v' : '^');
                    std::fill_n(back_inserter(move_buf), std::abs(dx),
                                dx > 0 ? '>' : '<');
                    move_buf.push_back('A');

                    self(pad_i + 1, p1_min_moves, p2_min_moves);
                    move_buf.resize(move_buf.size() - move_len);
                }

                x = x2;
                y = y2;

                p1_total_min_moves += p1_min_moves;
                p2_total_min_moves += p2_min_moves;
            }

            out_p1_min_moves = std::min(out_p1_min_moves, p1_total_min_moves);
            out_p2_min_moves = std::min(out_p2_min_moves, p2_total_min_moves);

            memo.emplace(memo_key,
                         std::pair{out_p1_min_moves, out_p2_min_moves});
        };

        memo.clear();
        auto p1_min_moves = std::numeric_limits<std::size_t>::max(),
             p2_min_moves = std::numeric_limits<std::size_t>::max();

        sum_min_moves(0, p1_min_moves, p2_min_moves);

        p1 += p1_min_moves * num_part;
        p2 += p2_min_moves * num_part;
    }

    if (!file.eof())
        std::abort();

    std::cout << "Day21: P1: " << p1 << ", P2: " << p2 << std::endl;
}
