#include <algorithm>
#include <charconv>
#include <cstdint>
#include <fstream>
#include <functional>
#include <iostream>
#include <print>
#include <string>
#include <vector>

int main()
{
    std::ifstream file("input");
    if (!file) {
        std::abort();
    }

    std::vector<std::uint32_t> grid;
    std::string cgrid;
    std::size_t cwidth = 0;
    std::size_t width = 0;
    std::size_t height = 0;

    std::string line;
    while (std::getline(file, line)) {
        if (cwidth > 0 && line.size() != cwidth) {
            std::abort();
        }
        cgrid += line;
        cwidth = line.size();

        std::size_t pushed = 0;
        std::size_t pos = 0;
        while (pos != std::string::npos) {
            const std::size_t start = line.find_first_not_of(' ', pos);
            if (start == std::string::npos) {
                break;
            }

            std::uint32_t value = 0;
            const auto [endp, ec] = std::from_chars(
                line.data() + start, line.data() + line.size(), value);
            if (ec != std::errc()) {
                if (pushed == 0) {
                    goto read_ops;
                }
                std::abort();
            }
            grid.push_back(value);
            ++pushed;
            pos = endp - line.data();
        }
        if (width > 0 && pushed != width) {
            std::abort();
        }

        width = pushed;
        ++height;
    }
    if (file.eof()) {
        std::abort();
    }

read_ops:
    const auto get_op = [](const char op_char)
        -> std::pair<std::uint64_t (*)(std::uint64_t, std::uint64_t),
                     std::uint64_t> {
        switch (op_char) {
        case '+':
            return {[](const std::uint64_t a, const std::uint64_t b) {
                        return a + b;
                    },
                    0};
        case '*':
            return {[](const std::uint64_t a, const std::uint64_t b) {
                        return a * b;
                    },
                    1};
        default:
            std::abort();
        }
    };

    std::uint64_t p1 = 0;
    std::size_t op_count = 0;
    std::size_t pos = 0;
    while (pos != std::string::npos) {
        const std::size_t op_pos = line.find_first_not_of(' ', pos);
        if (op_pos == std::string::npos) {
            break;
        }
        const auto [op_func, init_acc] = get_op(line[op_pos]);
        auto acc = init_acc;
        for (std::size_t y = 0; y < height; ++y) {
            acc = op_func(acc, grid[y * width + op_count]);
        }
        p1 += acc;
        ++op_count;
        pos = op_pos + 1;
    }
    if (op_count != width) {
        std::abort();
    }
    std::print("Day6: P1: {}", p1);

    std::uint64_t p2 = 0;
    const std::size_t cheight = height + 1;
    std::vector<std::uint64_t> nums;
    const auto idx = [&](const std::size_t x, const std::size_t y) {
        return y * cwidth + x;
    };
    const auto is_digit = [](const char c) { return c >= '0' && c <= '9'; };

    // rip cache and any semblance of optimization...
    for (std::size_t x = cwidth; x-- > 0;) {
        for (std::size_t y = 0; y < cheight; ++y) {
            char c = cgrid[idx(x, y)];
            if (c == ' ') {
                continue;
            }
            if (is_digit(c)) {
                std::uint64_t num = 0;
                for (; y < cheight; ++y) {
                    const char c2 = cgrid[idx(x, y)];
                    if (!is_digit(c2)) {
                        break;
                    }
                    num = num * 10 + (c2 - '0');
                }
                nums.push_back(num);
                --y;
                continue;
            }

            c = cgrid[idx(x, y)];
            const auto [op_func, init_acc] = get_op(c);
            p2 += std::ranges::fold_left(nums, init_acc, op_func);
            nums.clear();
        }
    }
    std::println(", P2: {}", p2);
}
