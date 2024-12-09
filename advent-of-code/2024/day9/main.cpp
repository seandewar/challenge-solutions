#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include <list>
#include <vector>

int main()
{
    std::ifstream file{"input"};
    if (!file)
        throw std::runtime_error{"failed to open input file"};

    std::string line;
    if (!std::getline(file, line))
        throw std::runtime_error{"failed to read input file"};
    if (!std::all_of(line.begin(), line.end(),
                     [](const char c) { return c >= '0' && c <= '9'; }))
        throw std::runtime_error{"bad input; all characters must be 0-9"};
    for (char c; (file >> c) && c == '\n';) {} // trim trailing NLs (cringe)
    if (!file.eof())
        throw std::runtime_error{"bad input; must be one line"};

    constexpr auto empty_block = std::numeric_limits<unsigned>::max();
    std::vector<unsigned> orig_blocks;
    for (std::size_t i = 0; i < line.size(); ++i) {
        for (unsigned j = line[i] - '0'; j > 0; --j)
            orig_blocks.push_back(i / 2);
        if (++i >= line.size())
            break;
        for (unsigned j = line[i] - '0'; j > 0; --j)
            orig_blocks.push_back(empty_block);
    }

    auto blocks = orig_blocks;
    for (std::size_t l = 0, r = blocks.size() - 1; l < r;) {
        if (blocks[r] == empty_block) {
            --r;
            continue;
        }
        if (blocks[l] != empty_block) {
            ++l;
            continue;
        }
        std::swap(blocks[l++], blocks[r--]);
    }

    std::size_t p1 = 0;
    for (std::size_t i = 0; i < blocks.size() && blocks[i] != empty_block; ++i)
        p1 += i * blocks[i];

    std::cout << "Day9: P1: " << p1 << std::flush;

    blocks = orig_blocks;
    std::list<std::pair<std::size_t, std::size_t>> free_list;
    for (std::size_t l = 0; l < blocks.size();) {
        auto r = l + 1;
        for (; r < blocks.size() && blocks[r - 1] == blocks[r]; ++r) {}
        if (blocks[l] == empty_block)
            free_list.emplace_back(l, r - l);
        l = r;
    }
    for (auto r = blocks.size(); r > 0;) {
        auto l = r - 1;
        for (; l > 0 && blocks[l - 1] == blocks[l]; --l) {}
        if (blocks[l] != empty_block) {
            const auto size = r - l;
            auto it = free_list.begin();
            for (; it != free_list.end() && it->first < l && it->second < size;
                 ++it) {}
            if (it != free_list.end() && it->first < l) {
                auto &[free_i, free_size] = *it;
                std::fill(blocks.begin() + free_i,
                          blocks.begin() + free_i + size, blocks[l]);
                std::fill(blocks.begin() + l, blocks.begin() + r, empty_block);
                if (free_size > size) {
                    free_i += size;
                    free_size -= size;
                } else {
                    it = free_list.erase(it);
                }

                for (; it != free_list.end() && it->first < l; ++it) {}
                const auto it_new = free_list.emplace(it, l, size);
                if (it != free_list.end() && it->first == r) {
                    it_new->second += it->second;
                    free_list.erase(it);
                }
                if (it_new != free_list.begin()) {
                    if (const auto it_prev = std::prev(it_new);
                        it_prev->first + it_prev->second == l) {
                        it_prev->second += it_new->second;
                        free_list.erase(it_new);
                    }
                }
            }
        }
        r = l;
    }

    std::size_t p2 = 0;
    for (std::size_t i = 0; i < blocks.size(); ++i)
        if (blocks[i] != empty_block)
            p2 += i * blocks[i];

    std::cout << ", P2: " << p2 << std::endl;
}
