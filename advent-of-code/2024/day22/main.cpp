#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int main()
{
    std::ifstream file{"input"};
    if (!file)
        std::abort();

    std::vector<unsigned> p2_price_sums;

    constexpr std::size_t p2_memo_cols = 19,
                          p2_memo_size = p2_memo_cols * p2_memo_cols
                                         * p2_memo_cols * p2_memo_cols,
                          no_index = std::numeric_limits<std::size_t>::max();

    std::vector p2_price_sum_is(p2_memo_size, no_index),
        p2_seen_changes(p2_memo_size, no_index);

    const auto p2_memo_index = [&](const std::array<int, 4> &changes) {
        assert(std::ranges::all_of(
            changes, [](const auto v) { return v >= -9 && v <= 9; }));

        const std::size_t i = changes[0] + 9, j = changes[1] + 9,
                          k = changes[2] + 9, l = changes[3] + 9;
        return (i * p2_memo_cols * p2_memo_cols * p2_memo_cols)
               + (j * p2_memo_cols * p2_memo_cols) + (k * p2_memo_cols) + l;
    };

    std::uint_least64_t p1 = 0;

    std::string line;
    for (std::size_t line_i = 0; std::getline(file, line); ++line_i) {
        std::uint_least32_t num;
        const auto [after_num, ec] =
            std::from_chars(line.data(), line.data() + line.size(), num);
        if (ec != std::errc{} || *after_num != '\0')
            std::abort();

        // Not bothering with circular buffering for only 4 elements.
        std::array<int, 4> p2_price_changes;
        int prev_p2_price;
        for (unsigned i = 0; i < 2000; ++i) {
            num ^= num << 6;
            num &= 0xffffff;

            num ^= num >> 5;
            num &= 0xffffff;

            num ^= num << 11;
            num &= 0xffffff;

            const int p2_price = num % 10;
            if (i > 0) {
                // This overlap is allowed, as std::copy copies forwards (it's
                // OK as the output iterator starts outside the source range).
                std::copy(p2_price_changes.begin() + 1, p2_price_changes.end(),
                          p2_price_changes.begin());
                p2_price_changes.back() = p2_price - prev_p2_price;
            }
            if (i >= 4) {
                const auto memo_i = p2_memo_index(p2_price_changes);

                if (p2_seen_changes[memo_i] != line_i) {
                    p2_seen_changes[memo_i] = line_i;

                    auto &sum_i = p2_price_sum_is[memo_i];
                    if (sum_i == no_index) {
                        sum_i = p2_price_sums.size();
                        p2_price_sums.push_back(0);
                    }

                    p2_price_sums[sum_i] += p2_price;
                }
            }

            prev_p2_price = p2_price;
        }

        p1 += num;
    }

    if (!file.eof())
        std::abort();

    std::cout << "Day22: P1: " << p1 << std::flush;

    const auto p2_it = std::ranges::max_element(p2_price_sums);
    std::cout << ", P2: " << (p2_it != p2_price_sums.end() ? *p2_it : 0)
              << std::endl;
}
