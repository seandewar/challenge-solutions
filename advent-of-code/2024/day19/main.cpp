#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <vector>

struct TrieNode {
    std::size_t i;
    std::array<std::unique_ptr<TrieNode>, 5> children;
    bool ends_word = false;

    TrieNode(const std::size_t i) : i(i) {}

    auto &child(this auto &self, const char c)
    {
        constexpr std::array chars{'r', 'g', 'b', 'u', 'w'};
        const auto it = std::find(chars.begin(), chars.end(), c);
        if (it == chars.end())
            std::abort();

        return self.children[it - chars.begin()];
    }
};

struct Trie {
    TrieNode root;
    std::size_t next_i;

    Trie() : root(0), next_i(1) {}

    void insert(std::string_view s)
    {
        auto node = &root;
        for (; !s.empty(); s.remove_prefix(1)) {
            auto &child = node->child(s.front());
            if (!child)
                child = std::make_unique<TrieNode>(next_i++);

            node = child.get();
        }

        node->ends_word = true;
    }
};

int main()
{
    std::ifstream file{"input"};
    if (!file)
        std::abort();

    std::string line;
    if (!std::getline(file, line))
        std::abort();

    Trie trie;
    for (std::size_t i = 0; i < line.size();) {
        const auto j = std::min(line.size(), line.find_first_of(", ", i));
        trie.insert({line.data() + i, j - i});
        i = j + 2;
    }

    std::string design_buf;
    std::vector<std::pair<std::size_t, std::size_t>> designs;
    while (std::getline(file, line)) {
        if (std::all_of(line.begin(), line.end(),
                        [](const char c) { return std::isspace(c); }))
            continue;

        designs.emplace_back(design_buf.size(), line.size());
        design_buf.append(line);
    }
    if (!file.eof())
        std::abort();

    std::vector<std::unordered_map<std::string_view, std::size_t>> memo(
        trie.next_i);

    const auto count_arrangements =
        [&](this const auto &self, const TrieNode &node,
            const std::string_view s) -> std::size_t {
        if (s.empty())
            return node.ends_word;

        auto &node_memo = memo[node.i];
        const auto it = node_memo.find(s);
        if (it != node_memo.end())
            return it->second;

        std::size_t count = 0;
        if (node.ends_word)
            count += self(trie.root, s);

        const auto &child = node.child(s.front());
        if (child)
            count += self(*child.get(), s.substr(1));

        return node_memo[s] = count;
    };

    std::size_t p1 = 0, p2 = 0;
    for (const auto [i, n] : designs) {
        const auto count =
            count_arrangements(trie.root, {design_buf.data() + i, n});

        if (count > 0)
            ++p1;
        p2 += count;
    }

    std::cout << "Day19: P1: " << p1 << ", P2: " << p2 << std::endl;
}
