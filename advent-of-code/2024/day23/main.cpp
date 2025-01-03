#include <algorithm>
#include <array>
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

    constexpr std::size_t max_names = 26 * 26;
    std::array<std::array<bool, max_names>, max_names> graph{};
    std::array<bool, max_names> has_node{};

    const auto name_index = [&](const std::string_view name) -> std::size_t {
        assert(name.size() == 2);
        if (!std::ranges::all_of(
                name, [](const auto c) { return c >= 'a' && c <= 'z'; }))
            std::abort();

        return (name[0] - 'a') * 26 + (name[1] - 'a');
    };

    for (std::string line; std::getline(file, line);) {
        if (line.size() != 5 || line[2] != '-')
            std::abort();

        const std::string_view a{line.data(), 2}, b{line.data() + 3, 2};
        const auto i = name_index(a), j = name_index(b);

        graph[i][j] = true;
        graph[j][i] = true;
        has_node[i] = true;
        has_node[j] = true;
    }

    if (!file.eof())
        std::abort();

    unsigned p1 = 0;

    constexpr auto ta_i = name_index("ta"), tz_i = name_index("tz");
    for (auto i = ta_i; i <= tz_i; ++i) {
        if (!has_node[i])
            continue;

        for (auto j = 0; j < max_names; ++j) {
            if (j >= ta_i && j <= i) {
                j = i; // Also avoid duplicate "tX" nodes.
                continue;
            }
            if (!graph[i][j])
                continue;

            for (auto k = j + 1; k < max_names; ++k) {
                if (k >= ta_i && k <= i) {
                    k = i; // Also avoid duplicate "tX" nodes.
                    continue;
                }
                if (!graph[j][k] || !graph[i][k])
                    continue;

                ++p1;
            }
        }
    }

    std::cout << "Day23: P1: " << p1 << std::flush;

    std::vector<std::size_t> largest_group, group_buf;
    const auto find_largest_group = [&](this const auto &self,
                                        const std::size_t i) {
        if (!std::ranges::all_of(group_buf,
                                 [&](const auto j) { return graph[i][j]; }))
            return;

        group_buf.push_back(i);

        for (std::size_t j = i + 1; j < max_names; ++j)
            if (graph[i][j])
                self(j);

        if (group_buf.size() > largest_group.size())
            largest_group = group_buf;

        group_buf.pop_back(); // back-track
    };

    for (std::size_t i = 0; i < max_names; ++i)
        if (has_node[i])
            find_largest_group(i);

    std::cout << ", P2: ";
    for (std::size_t i = 0; i < largest_group.size(); ++i) {
        if (i > 0)
            std::cout << ',';

        const auto node_i = largest_group[i];
        std::cout << static_cast<char>((node_i / 26) + 'a')
                  << static_cast<char>((node_i % 26) + 'a');
    }
    std::cout << std::endl;
}
