// Virtual machine time! (very festive)
#include <algorithm>
#include <charconv>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>

int main()
{
    std::ifstream file{"input"};
    if (!file)
        throw std::runtime_error{"failed to open input file"};

    std::string line;
    const auto parse_next_reg_line = [&] {
        if (!std::getline(file, line))
            throw std::runtime_error{"malformed input or read error"};
        const auto i = line.find(": ");
        if (i == std::string::npos)
            throw std::runtime_error{"malformed input"};

        std::uint_least64_t reg;
        const auto [next, ec] = std::from_chars(line.data() + i + 2,
                                                line.data() + line.size(), reg);
        if (ec != std::errc{})
            throw std::runtime_error{"malformed input"};
        return reg;
    };

    auto p1_init_a = parse_next_reg_line(), init_b = parse_next_reg_line(),
         init_c = parse_next_reg_line();

    for (; std::getline(file, line);)
        if (!std::all_of(line.begin(), line.end(),
                         [](const char c) { return std::isspace(c); }))
            break;

    std::vector<unsigned> program;
    {
        auto i = line.find(": ");
        if (i == std::string::npos)
            throw std::runtime_error{"malformed input"};

        for (i += 2; i < line.size();) {
            unsigned op;
            const auto [after, ec] =
                std::from_chars(line.data() + i, line.data() + line.size(), op);
            if (ec != std::errc{} || op > 0b111)
                throw std::runtime_error{"malformed input"};

            program.push_back(op);
            i = after - line.data() + 1;
        }
        if (program.size() % 2 != 0)
            throw std::runtime_error{"malformed input"};
    }

    for (; std::getline(file, line);)
        if (!std::all_of(line.begin(), line.end(),
                         [](const char c) { return std::isspace(c); }))
            break;
    if (!file.eof())
        throw std::runtime_error{"failed to read input file"};

    std::vector<unsigned> output;
    const auto run_program = [&](std::uint_least64_t a) {
        auto b = init_b, c = init_c;
        output.clear();

        for (unsigned pc = 0; pc + 1 < program.size(); pc += 2) {
            const auto opcode = program[pc], operand = program[pc + 1];
            const auto combo_operand = [&]() -> std::uint_least64_t {
                switch (operand) {
                case 4:
                    return a;
                case 5:
                    return b;
                case 6:
                    return c;
                case 7:
                    throw std::runtime_error{
                        "malformed input (reserved combo operand)"};
                default:
                    return operand;
                }
            };

            switch (opcode) {
            case 0: // adv
                a = a >> combo_operand();
                break;
            case 1: // bxl
                b = b ^ operand;
                break;
            case 2: // bst
                b = combo_operand() & 0b111;
                break;
            case 3: // jnz
                if (a != 0)
                    pc = operand - 2;
                break;
            case 4: // bxc
                b ^= c;
                break;
            case 5: // out
                output.push_back(combo_operand() & 0b111);
                break;
            case 6: // bdv
                b = a >> combo_operand();
                break;
            case 7: // cdv
                c = a >> combo_operand();
                break;
            }
        }
    };

    std::cout << "Day17: P1: ";
    run_program(p1_init_a);
    for (std::size_t i = 0; i < output.size(); ++i) {
        if (i > 0)
            std::cout << ',';
        std::cout << output[i];
    }
    std::cout << " P2: " << std::flush;

    // This relies on hashes of outputs with the same suffixes having consistent
    // hash suffixes, which seems to be the case for the example and my input.
    std::uint_least64_t p2 = 0;
    const auto p2_solve = [&](this const auto &self, const unsigned i) {
        if (i >= program.size())
            return true;

        for (unsigned x = 0; x < (1 << 3); ++x) {
            p2 <<= 3;
            p2 |= x;

            run_program(p2);
            if (std::equal(program.end() - 1 - i, program.end(), output.begin())
                && self(i + 1))
                return true;

            p2 >>= 3; // back-track
        }

        return false;
    };
    if (!p2_solve(0))
        throw std::runtime_error{"couldn't solve for P2"};

    std::cout << p2 << std::endl;
}
