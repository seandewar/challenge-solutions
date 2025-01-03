#include <array>
#include <cassert>
#include <climits>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int main()
{
    std::ifstream file{"input"};
    if (!file)
        std::abort();

    enum class InputState {
        Off,
        On,
        Unknown,
    };
    constexpr std::size_t max_names = 36 * 36 * 36;
    std::array<InputState, max_names> inputs;
    inputs.fill(InputState::Unknown);

    const auto name_index = [](const std::string_view s) {
        assert(s.size() == 3);

        const auto char_index = [](const char c) -> std::size_t {
            if (c >= 'a' && c <= 'z')
                return c - 'a';
            if (c >= '0' && c <= '9')
                return (c - '0') + 26;
            std::abort();
        };

        const auto i = char_index(s[0]), j = char_index(s[1]),
                   k = char_index(s[2]);
        return (i * 36 * 36) + (j * 36) + k;
    };

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty())
            break;
        if (line.size() != 6 || (line.back() != '0' && line.back() != '1'))
            std::abort();

        const auto i = name_index({line.data(), 3});
        inputs[i] = line.back() == '0' ? InputState::Off : InputState::On;
    }

    enum class Operation {
        And,
        Or,
        Xor,
    };
    struct Gate {
        std::size_t in_i, in_j;
        std::size_t out_i;
        Operation op;
    };
    std::vector<Gate> gates;
    std::array<std::vector<std::size_t>, max_names> gate_is_by_input_i;

    while (std::getline(file, line)) {
        if (line.size() != 17 && line.size() != 18)
            std::abort();

        Gate gate;
        gate.in_i = name_index({line.data(), 3});
        gate.in_j = name_index({line.data() + line.size() - 10, 3});
        gate.out_i = name_index({line.data() + line.size() - 3, 3});

        const std::string_view op_s{line.data() + 4, 4};
        if (op_s == "XOR ")
            gate.op = Operation::Xor;
        else if (op_s == "AND ")
            gate.op = Operation::And;
        else if (op_s.starts_with("OR "))
            gate.op = Operation::Or;
        else
            std::abort();

        gate_is_by_input_i[gate.in_i].push_back(gates.size());
        gate_is_by_input_i[gate.in_j].push_back(gates.size());

        gates.push_back(gate);
    }

    if (!file.eof())
        std::abort();

    const auto simulate_gate = [&](this const auto &self, const std::size_t i) {
        const auto &gate = gates[i];
        if (inputs[gate.in_i] == InputState::Unknown
            || inputs[gate.in_j] == InputState::Unknown
            || inputs[gate.out_i] != InputState::Unknown)
            return;

        const bool in_a = inputs[gate.in_i] == InputState::On,
                   in_b = inputs[gate.in_j] == InputState::On;

        bool out;
        switch (gate.op) {
        case Operation::And:
            out = in_a && in_b;
            break;
        case Operation::Or:
            out = in_a || in_b;
            break;
        case Operation::Xor:
            out = in_a != in_b;
            break;
        }

        inputs[gate.out_i] = out ? InputState::On : InputState::Off;
        for (const auto j : gate_is_by_input_i[gate.out_i])
            self(j);
    };
    for (std::size_t i = 0; i < gates.size(); ++i)
        simulate_gate(i);

    std::uintmax_t p1 = 0;
    std::array name_buf{'z', '0', '0'};
    for (unsigned i = 100; i-- > 0;) {
        name_buf[1] = (i / 10) + '0';
        name_buf[2] = (i % 10) + '0';

        const auto in = inputs[name_index({name_buf.data(), 3})];
        if (i >= CHAR_BIT * sizeof p1) {
            // We rely on std::uintmax_t having enough bits to represent the
            // answer; go boom if that's not true.
            if (in == InputState::On)
                std::abort();
            continue;
        }

        p1 <<= 1;
        p1 |= in == InputState::On;
    }

    std::cout << "Day24: P1: " << p1 << std::endl;
}
