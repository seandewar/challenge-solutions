#include <cassert>
#include <charconv>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <span>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

struct MemoKey {
    std::uint8_t springs_len, lens_len;
    unsigned len;
    bool spring_prev;
    bool operator==(const MemoKey &) const = default;
};

template<>
struct std::hash<MemoKey> {
    size_t operator()(const MemoKey &s) const {
        return static_cast<std::size_t>(s.len) << 17 | s.springs_len
               | static_cast<std::size_t>(s.lens_len) << 8
               | static_cast<std::size_t>(s.spring_prev) << 16;
    }
};

using Memo = std::unordered_map<MemoKey, uint_least64_t>;

static std::uint_least64_t dfs(Memo &memo, const std::string_view springs,
                               const std::span<unsigned> lens,
                               const bool spring_prev = false) {
    if (springs.empty())
        return lens.empty() || (lens.size() == 1 && lens[0] == 0);

    const MemoKey memo_key{
        .springs_len = static_cast<std::uint8_t>(springs.size()),
        .lens_len = static_cast<std::uint8_t>(lens.size()),
        .len = !lens.empty() ? lens[0] : 0,
        .spring_prev = spring_prev,
    };
    if (const auto entry = memo.find(memo_key); entry != memo.cend())
        return entry->second;

    const auto spring = [&]() -> std::uint_least64_t {
        if (lens.empty() || lens[0] == 0)
            return 0;
        --lens[0];
        const auto result = dfs(memo, springs.substr(1), lens, true);
        ++lens[0];
        return result;
    };
    const auto empty = [&]() -> std::uint_least64_t {
        if (spring_prev && lens[0] != 0)
            return 0;
        return dfs(memo, springs.substr(1),
                   !lens.empty() && lens[0] == 0 ? lens.subspan(1) : lens);
    };

    const char c = springs[0];
    return memo[memo_key] = c == '?'   ? spring() + empty()
                            : c == '#' ? spring()
                                       : empty();
}

int main() {
    std::ifstream f{"input"};
    assert(f.is_open());

    std::uint_least64_t p1 = 0, p2 = 0;
    Memo memo;
    std::vector<unsigned> lens;
    for (std::string line; std::getline(f, line);) {
        const auto space_i = line.find(' ');
        assert(space_i != std::string::npos);
        lens.clear();
        for (auto i = space_i + 1; i < line.size();) {
            const auto j = std::min(line.size(), line.find(',', i + 1));
            unsigned len;
            const auto res =
                std::from_chars(line.data() + i, line.data() + j, len);
            assert(res.ec == std::errc{} && len != 0);
            lens.push_back(len);
            if (j == line.size())
                break;
            i = j + 1;
        }
        line.erase(space_i);
        memo.clear();
        p1 += dfs(memo, line, lens);

        line.reserve(line.size() * 5 + 4);
        lens.reserve(lens.size() * 5);
        const auto line_end = line.cend();
        const auto lens_end = lens.cend();
        for (int i = 0; i < 4; ++i) {
            line += '?';
            std::copy(line.cbegin(), line_end, std::back_inserter(line));
            std::copy(lens.cbegin(), lens_end, std::back_inserter(lens));
        }
        p2 += dfs(memo, line, lens);
    }
    assert(f.eof());

    std::cout << "Day12: P1: " << p1 << ", P2: " << p2 << '\n';
}
