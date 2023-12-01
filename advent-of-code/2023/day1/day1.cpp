#include <algorithm>
#include <array>
#include <cassert>
#include <fstream>
#include <iostream>
#include <string_view>

int main()
{
    std::ifstream file{"input"};
    int p1 = 0, p2 = 0;

    for (std::string line; std::getline(file, line);) {
        constexpr auto is_digit = [](const char c) { return c >= '0' && c <= '9'; };
        const auto l_digit_it = std::ranges::find_if(line, is_digit);
        const auto r_digit_it = std::find_if(line.rbegin(), line.rend(), is_digit);

        auto l_it = l_digit_it;
        auto r_it = r_digit_it;
        int l_digit, r_digit;

        if (l_it != line.end()) {
            l_digit = *l_it - '0';
            r_digit = *r_it - '0';
            p1 += l_digit * 10 + r_digit;
        }

        constexpr std::array<std::string_view, 9> words{
            "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};
        for (int digit = 1; digit <= 9; ++digit) {
            const auto w = words[digit - 1];
            const auto l = std::search(line.begin(), l_digit_it, w.begin(), w.end());
            const auto r = std::search(line.rbegin(), r_digit_it, w.rbegin(), w.rend());

            if (l < l_it) {
                l_it = l;
                l_digit = digit;
            }
            if (r < r_it) {
                r_it = r;
                r_digit = digit;
            }
        }

        assert(l_it != line.end());
        p2 += l_digit * 10 + r_digit;
    }

    assert(file.eof());
    std::cout << "Day1: P1: " << p1 << ", P2: " << p2 << '\n';
}
