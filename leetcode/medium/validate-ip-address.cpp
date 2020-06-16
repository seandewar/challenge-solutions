// https://leetcode.com/problems/validate-ip-address/
//
// Fully-regex solution.
//
// You could split the regex up into the match required for one IPvX number,
// constructing the full regex at runtime or statically instead (see alternative
// solution), but it's easier to use the magic of vim instead. :)
//
// Complexity: runtime Omega(n) [n == size(ip)].

class Solution {
public:
    string validIPAddress(const string& ip) const
    {
        // avoids matching trailing 0s, and ensures numbers are between 0 & 255
        const regex v4("((?:[1-9]?\\d)|(?:1\\d\\d)|(?:2[0-4]\\d)|(?:25[0-5]))\."
                       "((?:[1-9]?\\d)|(?:1\\d\\d)|(?:2[0-4]\\d)|(?:25[0-5]))\."
                       "((?:[1-9]?\\d)|(?:1\\d\\d)|(?:2[0-4]\\d)|(?:25[0-5]))\."
                       "((?:[1-9]?\\d)|(?:1\\d\\d)|(?:2[0-4]\\d)|(?:25[0-5]))");

        // expects 1 to 4 hexadecimal digits per number (trailing 0s allowed)
        const regex v6("([\\da-fA-F]{1,4}):([\\da-fA-F]{1,4}):"
                       "([\\da-fA-F]{1,4}):([\\da-fA-F]{1,4}):"
                       "([\\da-fA-F]{1,4}):([\\da-fA-F]{1,4}):"
                       "([\\da-fA-F]{1,4}):([\\da-fA-F]{1,4})");

        // change the rest into a one-line nested ternary if you're chaotic :)
        if (regex_match(ip, v4))
            return "IPv4";
        else if (regex_match(ip, v6))
            return "IPv6";

        return "Neither";
    }
};

// Alternative Solution: Chunked version of the above solution.
//
// The regex string is reconstructed statically via use of the pre-processor.
// With C++20, you can use constexpr strings instead.
//
// Nested ternary is included for fun.
//
// Complexity: runtime Omega(n) [n == size(ip)].

#define V4_CHUNK "((?:[1-9]?\\d)|(?:1\\d\\d)|(?:2[0-4]\\d)|(?:25[0-5]))"
#define V6_CHUNK "([\\da-fA-F]{1,4})"

class Solution {
public:
    string validIPAddress(const string& ip) const
    {
        const regex v4(V4_CHUNK "\." V4_CHUNK "\." V4_CHUNK "\." V4_CHUNK);
        const regex v6(V6_CHUNK ":" V6_CHUNK ":" V6_CHUNK ":" V6_CHUNK ":"
                       V6_CHUNK ":" V6_CHUNK ":" V6_CHUNK ":" V6_CHUNK);

        return regex_match(ip, v4) ? "IPv4"
                                   : (regex_match(ip, v6) ? "IPv6" : "Neither");
    }
};

// Alternative Solution: Full-regex solution for IPv6, but partial for IPv4;
// checks IPv4 numbers < 256 manually instead to simplify its regex a bit.
//
// Complexity: runtime Omega(n) [n == size(ip)].

class Solution {
public:
    string validIPAddress(const string& ip) const
    {
        // avoids matching trailing 0s, and ensures numbers are between 0 & 299;
        // we will need to check at runtime later that they are < 256
        const regex v4("((?:[1-9]?\\d)|(?:[12]\\d\\d))\."
                       "((?:[1-9]?\\d)|(?:[12]\\d\\d))\."
                       "((?:[1-9]?\\d)|(?:[12]\\d\\d))\."
                       "((?:[1-9]?\\d)|(?:[12]\\d\\d))");

        // expects 1 to 4 hexadecimal digits per number (trailing 0s allowed)
        const regex v6("([\\da-fA-F]{1,4}):([\\da-fA-F]{1,4}):"
                       "([\\da-fA-F]{1,4}):([\\da-fA-F]{1,4}):"
                       "([\\da-fA-F]{1,4}):([\\da-fA-F]{1,4}):"
                       "([\\da-fA-F]{1,4}):([\\da-fA-F]{1,4})");

        if (smatch match; regex_match(ip, match, v4)) {
            // check that the IPv4 numbers are < 256.
            // NOTE: 1st submatch is always the full match; skip with
            // cbegin(match) + 1
            if (all_of(cbegin(match) + 1, cend(match), [](const auto& subm) {
                    return stoul(subm.str()) < 256;
                })) {
                return "IPv4";
            }
        }
        else if (regex_match(ip, v6)) {
            return "IPv6";
        }

        return "Neither";
    }
};
