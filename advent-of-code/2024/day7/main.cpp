#include <charconv>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int main()
{
    std::ifstream file{"input"};
    if (!file)
        throw std::runtime_error{"failed to open input file"};

    std::uint64_t p1 = 0, p2 = 0;
    std::vector<unsigned> nums;
    for (std::string line; std::getline(file, line);) {
        std::uint64_t test_val;
        const auto [after_test_val, test_val_ec] =
            std::from_chars(line.data(), line.data() + line.size(), test_val);
        if (test_val_ec != std::errc{}
            || std::strncmp(after_test_val, ": ", 2) != 0)
            throw std::runtime_error{"bad input"};

        nums.clear();
        for (const char *p = after_test_val + 1; *p != '\0';) {
            if (*(p++) != ' ')
                throw std::runtime_error{"bad input"};

            unsigned num;
            const auto [after_num, num_ec] =
                std::from_chars(p, line.data() + line.size(), num);
            if (num_ec != std::errc{})
                throw std::runtime_error{"bad input"};

            nums.push_back(num);
            p = after_num;
        }

        enum class TestResult {
            Invalid,
            Valid,
            ValidWithConcat,
        };
        // memoization considered cringe
        const auto run_test = [&](this const auto &self,
                                  const std::uint64_t sum, const size_t i,
                                  const bool used_concat) -> TestResult {
            if (i >= nums.size() && sum == test_val)
                return used_concat ? TestResult::ValidWithConcat
                                   : TestResult::Valid;
            if (i >= nums.size() || sum > test_val)
                return TestResult::Invalid;

            const auto add_res = self(sum + nums[i], i + 1, used_concat);
            if (add_res == TestResult::Valid)
                return TestResult::Valid;

            const auto mul_res = self(sum * nums[i], i + 1, used_concat);
            if (mul_res == TestResult::Valid)
                return TestResult::Valid;
            if (add_res == TestResult::ValidWithConcat
                || mul_res == TestResult::ValidWithConcat)
                return TestResult::ValidWithConcat;

            std::uint64_t shifted_sum = sum;
            for (auto x = nums[i]; x > 0; x /= 10)
                shifted_sum *= 10;
            return self(shifted_sum + nums[i], i + 1, true);
        };

        const auto res = run_test(nums[0], 1, false);
        if (res == TestResult::Valid)
            p1 += test_val;
        if (res != TestResult::Invalid)
            p2 += test_val;
    }

    if (!file.eof())
        throw std::runtime_error{"failed to read input file"};

    std::cout << "Day7 P1: " << p1 << ", P2: " << p2 << std::endl;
}
