#include <algorithm>
#include <charconv>
#include <fstream>
#include <print>
#include <ranges>
#include <span>
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
    std::uint64_t p2 = 0;

    std::string num_buf;
    for (std::size_t start = 0; start < line.size();) {
        std::size_t end = line.find(',', start);
        if (end == std::string::npos)
            end = line.size();

        const std::string_view entry(&line[start], end - start);
        const std::size_t dash_pos = entry.find('-');
        if (dash_pos == std::string::npos)
            std::abort();
        const std::string_view a = entry.substr(0, dash_pos);
        const std::string_view b = entry.substr(dash_pos + 1);

        std::uint64_t a_int = 0;
        std::uint64_t b_int = 0;
        {
            const auto [pa, aec] =
                std::from_chars(a.data(), a.data() + a.size(), a_int);
            if (aec != std::errc() || pa != a.data() + a.size())
                std::abort();

            const auto [pb, bec] =
                std::from_chars(b.data(), b.data() + b.size(), b_int);
            if (bec != std::errc() || pb != b.data() + b.size())
                std::abort();
        }

        num_buf.clear();
        num_buf.resize(b.size(), '0');
        std::size_t num_start = num_buf.size() - a.size();

        const auto num_span = [&] -> std::span<char> {
            return {num_buf.begin() + num_start, num_buf.end()};
        };
        std::ranges::copy(a, num_span().begin());

        for (std::uint64_t num_int = a_int; num_int <= b_int; ++num_int) {
            const std::span<char> num_sp = num_span();

            for (std::size_t div = 2; div <= num_sp.size(); ++div) {
                if (num_sp.size() % div != 0)
                    continue;

                const std::span<char> l =
                    num_sp.subspan(0, num_sp.size() / div);

                std::size_t i = l.size();
                for (; i < num_sp.size(); i += l.size()) {
                    const std::span<char> r = num_sp.subspan(i, l.size());
                    if (!std::ranges::equal(l, r))
                        break;
                }

                if (i >= num_sp.size()) {
                    if (div == 2)
                        p1 += num_int;
                    p2 += num_int;
                    break;
                }
            }

            for (std::size_t i = num_buf.size(); i-- > 0;) {
                char &c = num_buf[i];
                if (c++ < '9')
                    break;

                c = '0';
                if (i == num_start)
                    --num_start;
            }
        }

        start = end + 1;
    }

    std::println("Day2: P1: {}, P2: {}", p1, p2);
}
