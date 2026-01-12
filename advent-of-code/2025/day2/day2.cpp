#include <algorithm>
#include <fstream>
#include <print>
#include <ranges>
#include <string>
#include <string_view>

int main()
{
    std::string line;
    {
        std::ifstream file("input");
        if (!file)
            std::abort();
        std::getline(file, line);
    }

    std::uint64_t p1 = 0;
    std::string num_buf;

    for (std::size_t start = 0; start < line.size();) {
        std::size_t end = line.find(',', start);
        if (end == std::string::npos)
            end = line.size();

        const std::string_view entry(&line[start], end - start);
        const std::size_t dash_pos = entry.find('-');
        if (dash_pos == std::string::npos)
            std::abort();
        const std::string_view a_str = entry.substr(0, dash_pos);
        const std::string_view b_str = entry.substr(dash_pos + 1);

        const auto non_digit = [](const char c) { return c < '0' || c > '9'; };
        if (std::ranges::any_of(a_str, non_digit)
            || std::ranges::any_of(b_str, non_digit)
            || (a_str.size() > 1 && a_str[0] == '0')
            || (b_str.size() > 1 && b_str[0] == '0'))
            std::abort();

        // Prepare num_buf to hold half of the digits of each checked number.
        // num_pos points to the most significant digit in num_buf.
        num_buf.clear();
        num_buf.resize((b_str.size() + 1) / 2, '0');

        std::size_t num_pos = 0;
        const auto incr_num_buf = [&] {
            for (char &c : num_buf | std::views::reverse) {
                if (c++ < '9')
                    return;
                c = '0';
            }
        };
        const auto num_view = [&] -> std::string_view {
            return {num_buf.begin() + num_pos, num_buf.end()};
        };
        const auto buf_to_num = [&] -> std::uint64_t {
            std::uint64_t num = 0;
            for (const char c :
                 std::views::repeat(num_view(), 2) | std::views::join)
                num = num * 10 + (c - '0');
            return num;
        };
        const auto num_gt_b = [&] -> bool {
            const std::string_view view = num_view();
            const std::string_view b_left = b_str.substr(0, view.size());
            const std::string_view b_right = b_str.substr(view.size());

            return (
                view.size() * 2 > b_str.size()
                || (view.size() * 2 == b_str.size()
                    && (view > b_left || (view == b_left && view > b_right))));
        };

        // Start num_buf with the most significant of a_str's digits. If a_str
        // can't be split evenly, start as the lowest "half" number with one
        // more digit. If duplicating num_buf would make a smaller number than
        // a_str, increment num_buf.
        num_pos = num_buf.size() - (a_str.size() / 2);
        if (a_str.size() % 2 != 0) {
            num_buf[--num_pos] = '1';
        } else {
            std::copy_n(a_str.begin(), a_str.size() / 2,
                        num_buf.begin() + num_pos);
            const std::string_view view = num_view();
            if (view < a_str.substr(view.size()))
                incr_num_buf();
        }

        std::println("{}-{}", a_str, b_str);
        if (num_gt_b()) {
            start = end + 1;
            continue;
        }

        while (true) {
            p1 += buf_to_num();
            std::string_view view = num_view();
            std::println("  {}{}", view, view);

            if (std::ranges::all_of(view,
                                    [](const char c) { return c == '9'; })) {
                if (num_pos == 0)
                    break;

                std::ranges::fill(num_buf, '0');
                num_buf[--num_pos] = '1';
                view = num_view();
            } else {
                incr_num_buf();
            }

            if (num_gt_b())
                break;
        }

        start = end + 1;
    }

    std::println("Day2: P1: {}", p1);
}
