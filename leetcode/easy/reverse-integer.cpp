// https://leetcode.com/problems/reverse-integer/
//
// Complexity: runtime O(log(x)), space O(1).

class Solution {
public:
    int reverse(int x)
    {
        // checking for overflow can be done without using a larger data type
        // than int (as suggested by the question), but it's unnecessarily
        // time-consuming for a leetcode easy difficulty question such as this.
        //
        // performing the calculation and checking for overflow by comparing it
        // with the previous value (e.g. is the new value much smaller/bigger
        // than expected?) is naughty as it forces the overflow anyway, which
        // is undefined behaviour for signed integer types!
        long long result{};

        while (x != 0) {
            result = (result * 10) + (x % 10);

            if (result > numeric_limits<int>::max()
                || result < numeric_limits<int>::min())
                return 0;

            x /= 10;
        }

        return static_cast<int>(result);
    }
};

// Cheesy solution using string manipulation. Costs extra space due to string
// allocation.
// Complexity: runtime is presumably O(log(x)), space O(log(x)).
/*
class Solution {
public:
    int reverse(int x)
    {
        try {
            if (x == numeric_limits<int>::min())
                throw out_of_range("int too small to represent abs(INT_MIN)");

            auto str = to_string(abs(x));
            std::reverse(begin(str), end(str));
            return stoi(str) * (x < 0 ? -1 : 1);
        }
        catch (const out_of_range&) {
            return 0; // abs(int) or stoi(reversed) too large to fit in an int
        }
    }
};
*/
