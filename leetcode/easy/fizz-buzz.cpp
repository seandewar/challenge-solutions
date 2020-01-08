// https://leetcode.com/problems/fizz-buzz/
//
// A classic; there are many ways of doing this. Here's an alternative:
//
// if (i % 15 == 0) // equivalent to (i%3==0 && i%5==0)
//     result.emplace_back("FizzBuzz");
// else if (i % 3 == 0)
//     result.emplace_back("Fizz");
// else if (i % 5 == 0)
//     result.emplace_back("Buzz");
// else
//     result.push_back(to_string(i));
//
// will also suffice, although there is an extra modulus calculation for the
// "FizzBuzz" case in comparison.
//
// Complexity: runtime O(n), space O(1) [output space is never considered].

class Solution {
public:
    vector<string> fizzBuzz(int n)
    {
        vector<string> result;
        result.reserve(n);

        for (int i = 1; i <= n; ++i) {
            string str;
            if (i % 3 == 0)
                str += "Fizz";
            if (i % 5 == 0)
                str += "Buzz";

            result.push_back(empty(str) ? to_string(i) : str);
        }

        return result;
    }
};

// Alternative Solution: no modulus calculations. Integer modulus calculations
// are similar in cost to integer divisions, which can be somewhat expensive if
// you're doing lots and lots of them (unless you're dealing with powers of 2).
//
// Alternative function body, less code:
//
// vector<string> result(n);
//
// for (int i = 2; i < n; i += 3)
//     result[i] += "Fizz";
// for (int i = 4; i < n; i += 5)
//     result[i] += "Buzz";
// for (int i = 0; i < n; ++i) {
//     if (empty(result[i]))
//         result[i] = to_string(i + 1);
// }
//
// return result;
//
// Complexity: runtime O(n), space O(1) [output space is never considered].
/*
class Solution {
public:
    vector<string> fizzBuzz(int n)
    {
        vector<string> result;
        result.reserve(n);

        for (int i = 1; i <= n; ++i)
            result.push_back(to_string(i));
        for (int i = 2; i < n; i += 3)
            result[i] = "Fizz";
        for (int i = 4; i < n; i += 5)
            result[i] = "Buzz";
        for (int i = 14; i < n; i += 15)
            result[i] = "FizzBuzz";

        return result;
    }
};
*/

// Fun Solution: no modulus calculations + compile-time FizzBuzz lookup table
// generation.
//
// Complexity: runtime O(n), space O(1) [output space is never considered].
//
// Alternatively, with a bit of work (and maybe a change in the question
// skeleton code + use of constexpr vector & string [C++20!]), you can
// completely generate the result buffer at compile time, producing a runtime
// of O(1)!
/*
class Solution {
    enum class FizzBuzzType {
        None,
        Fizz,
        Buzz,
        FizzBuzz
    };

    // ideally, no need to generate lookup entries for more than the first 15,
    // as the FizzBuzz pattern will loop afterwards. if you want to, you could
    // just hard-code the table instead... :)
    template <size_t N = 15>
    static constexpr auto generateFizzBuzzLookupTable()
    {
        array<FizzBuzzType, N> result{};
        for (size_t i = 2; i < N; i += 3)
            result[i] = FizzBuzzType::Fizz;
        for (size_t i = 4; i < N; i += 5)
            result[i] = FizzBuzzType::Buzz;
        for (size_t i = 14; i < N; i += 15)
            result[i] = FizzBuzzType::FizzBuzz;

        return result;
    }

public:
    vector<string> fizzBuzz(int n)
    {
        constexpr auto lookup = generateFizzBuzzLookupTable();

        vector<string> result;
        result.reserve(n);

        for (int i = 0, j = 0; i < n; ++i, j = (j >= 14 ? 0 : j + 1)) {
            switch (lookup[j]) { // may use [i % 15] if you want to use modulo
            case FizzBuzzType::Fizz:
                result.emplace_back("Fizz");
                break;
            case FizzBuzzType::Buzz:
                result.emplace_back("Buzz");
                break;
            case FizzBuzzType::FizzBuzz:
                result.emplace_back("FizzBuzz");
                break;
            default:
                result.push_back(to_string(i + 1));
            }
        }

        return result;
    }
};
*/
