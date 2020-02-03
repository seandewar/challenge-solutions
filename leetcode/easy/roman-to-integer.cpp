// https://leetcode.com/problems/roman-to-integer/
//
// See the Java Roman to Integer solution for comments.
// Complexity: runtime O(n), space O(1).

class Solution {
public:
    int romanToInt(const string s)
    {
        // NOTE: pair is specified for the first argument of the initializer so
        // that the compiler can correctly deduce this as an
        // unordered_map<char, int> via the ctor taking
        // initializer_list<pair<char, int>> using C++17 template arg deduction
        static const unordered_map romanMap{
            pair{'I', 1},
            {'V', 5},
            {'X', 10},
            {'L', 50},
            {'C', 100},
            {'D', 500},
            {'M', 1000}
        };

        int result{};

        for (string::size_type i{}; i < size(s); ++i) {
            const int value{romanMap.at(s[i])};
            const int nextValue{i < size(s) - 1 ? romanMap.at(s[i + 1]) : 0};

            result += value * (nextValue > value ? -1 : 1);
        }

        return result;
    }
};
