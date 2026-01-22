#include <algorithm>
#include <charconv>
#include <fstream>
#include <functional>
#include <limits>
#include <numeric>
#include <print>
#include <ranges>
#include <string>
#include <utility>
#include <vector>

int main()
{
    std::ifstream f("input");
    if (!f) {
        std::abort();
    }

    constexpr auto no_circuit = std::numeric_limits<std::size_t>::max();
    struct Pos {
        std::uint64_t x, y, z;
        std::size_t circuit_i = no_circuit;
    };
    std::vector<Pos> poses;
    std::string line;
    while (std::getline(f, line)) {
        Pos pos;
        const char *p = line.data();
        const auto read_num = [&](auto &x, const bool last = false) {
            const auto [end, ec] =
                std::from_chars(p, line.data() + line.size(), x);
            const char delim = last ? '\0' : ',';
            if (ec != std::errc{} || *end != delim) {
                std::abort();
            }
            if (!last) {
                p = end + 1;
            }
        };

        read_num(pos.x);
        read_num(pos.y);
        read_num(pos.z, true);
        poses.push_back(pos);
    }
    if (!f.eof() || poses.empty()) {
        std::abort();
    }

    struct Dist {
        std::uint64_t dd;
        std::size_t i, j;
    };
    std::vector<Dist> dists;
    dists.reserve(poses.size() * poses.size());
    for (std::size_t i = 0; i + 1 < poses.size(); ++i) {
        for (std::size_t j = i + 1; j < poses.size(); ++j) {
            const Pos &a = poses[i];
            const Pos &b = poses[j];
            const auto dd = ((a.x - b.x) * (a.x - b.x))
                            + ((a.y - b.y) * (a.y - b.y))
                            + ((a.z - b.z) * (a.z - b.z));
            dists.emplace_back(dd, i, j);
        }
    }

    constexpr std::size_t connections = 1000;
    if (dists.size() < connections) {
        std::abort();
    }
    std::ranges::sort(dists,
                      [](const auto &a, const auto &b) { return a.dd < b.dd; });

    std::vector<std::size_t> circuit_sizes;
    for (const auto &[i, d] : dists | std::views::enumerate) {
        Pos &a = poses[d.i];
        Pos &b = poses[d.j];
        if (a.circuit_i == no_circuit && b.circuit_i == no_circuit) {
            a.circuit_i = circuit_sizes.size();
            b.circuit_i = circuit_sizes.size();
            circuit_sizes.push_back(2);
        } else if (a.circuit_i == no_circuit || b.circuit_i == no_circuit) {
            const auto circuit_i = std::min(a.circuit_i, b.circuit_i);
            a.circuit_i = circuit_i;
            b.circuit_i = circuit_i;
            ++circuit_sizes[circuit_i];
        } else if (a.circuit_i != b.circuit_i) {
            const auto b_circuit_i = b.circuit_i;
            for (auto &p : poses) { // This is fast enough.
                if (p.circuit_i == b_circuit_i) {
                    p.circuit_i = a.circuit_i;
                }
            }
            circuit_sizes[a.circuit_i] +=
                std::exchange(circuit_sizes[b_circuit_i], 0);
        }

        if (i + 1 == connections) {
            std::array<std::size_t, 3> sizes;
            sizes.fill(1);
            std::ranges::partial_sort_copy(circuit_sizes, sizes,
                                           std::greater{});
            std::print("Day8: P1: {}", sizes[0] * sizes[1] * sizes[2]);
        }
        if (std::max(circuit_sizes[a.circuit_i], circuit_sizes[b.circuit_i])
            == poses.size()) {
            std::println(", P2: {}", a.x * b.x);
            return EXIT_SUCCESS;
        }
    }
    std::abort();
}
