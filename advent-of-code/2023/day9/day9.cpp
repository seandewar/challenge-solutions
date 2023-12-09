#include <algorithm>
#include <cassert>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <numeric>
#include <span>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

int main() {
    std::ifstream f{"input"};
    assert(f.is_open());

    std::int_least64_t p1 = 0, p2 = 0;
    std::vector<std::int_least32_t> nums;
    for (std::string line; std::getline(f, line);) {
        nums.clear();
        std::istringstream iss{line};
        std::copy(std::istream_iterator<std::int_least32_t>{iss},
                  std::istream_iterator<std::int_least32_t>{},
                  std::back_inserter(nums));

        std::span span{nums};
        for (; std::ranges::any_of(span, [](const auto x) { return x != 0; });
             span = span.last(span.size() - 1)) {
            assert(span.size() > 1);

            auto prev = span[0];
            for (size_t i = 1; i < span.size(); ++i)
                prev = std::exchange(span[i], span[i] - prev);

            p1 += prev;
        }

        p2 += std::accumulate(std::reverse_iterator{nums.cend() - span.size()},
                              nums.crend(), 0,
                              [](const auto a, const auto x) { return x - a; });
    }

    assert(f.eof());
    std::cout << "Day9: P1: " << p1 << " P2: " << p2 << '\n';
}
