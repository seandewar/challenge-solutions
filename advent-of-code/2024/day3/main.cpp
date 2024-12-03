#include <charconv>
#include <fstream>
#include <iostream>
#include <sstream>

int main()
{
    std::string input;
    {
        std::ostringstream oss;
        std::ifstream file{"input"};
        if (!file)
            throw std::runtime_error{"failed to open input file"};
        if (!(oss << file.rdbuf()))
            throw std::runtime_error{"failed to read input file"};
        input = oss.str();
    }

    unsigned long long p1 = 0, p2 = 0;
    bool p2_mul_enabled = true;
    for (size_t i = 0;; ++i) {
        i = input.find_first_of('(', i);
        if (i >= input.size())
            break;

        if (const std::string_view prefix{input.data(), i};
            !prefix.ends_with("mul")) {
            if ((prefix.ends_with("do") || prefix.ends_with("don't"))
                && input[i + 1] == ')') {
                p2_mul_enabled = prefix.ends_with("do");
                ++i;
            }
            continue;
        }

        unsigned a;
        const auto [a_end, a_errc] = std::from_chars(
            input.data() + i + 1, input.data() + input.size(), a);
        i = a_end - input.data();
        if (a_errc != std::errc{} || *a_end != ',')
            continue;

        unsigned b;
        const auto [b_end, b_errc] = std::from_chars(
            input.data() + i + 1, input.data() + input.size(), b);
        i = b_end - input.data();
        if (b_errc != std::errc{} || *b_end != ')')
            continue;

        p1 += static_cast<unsigned long long>(a) * b;
        if (p2_mul_enabled)
            p2 += static_cast<unsigned long long>(a) * b;
    }

    std::cout << "Day3: P1: " << p1 << ", P2: " << p2 << std::endl;
}
