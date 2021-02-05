// https://leetcode.com/problems/simplify-path
//
// Using std::string, std::(o&i)stringstream & std::getline().
// Complexity: runtime O(n), space O(n).

class Solution {
public:
    auto simplifyPath(const string& path) const -> string
    {
        vector<string> dirStack;
        istringstream iss(path);
        for (string dir; getline(iss, dir, '/');) {
            if (empty(dir) || dir == ".")
                continue;

            if (dir == "..") {
                if (!empty(dirStack))
                    dirStack.pop_back();

                continue;
            }

            dirStack.push_back(move(dir));
        }

        ostringstream oss;
        for (const auto& dir : dirStack)
            oss << '/' << dir;

        const auto result = oss.str();
        return empty(result) ? "/" : result;
    }
};

// Alternative Solution: Using std::string_view & std::ostringstream.
// Complexity: runtime O(n), space O(n).

class Solution {
public:
    auto simplifyPath(const string& path) const -> string
    {
        vector<string_view> dirStack;
        for (string_view rem(path); !empty(rem);) {
            const auto sepIdx = rem.find_first_of('/');
            const auto dir = rem.substr(0, sepIdx);
            rem.remove_prefix(sepIdx != string_view::npos ? sepIdx + 1
                                                          : size(rem));

            if (empty(dir) || dir == ".")
                continue;

            if (dir == "..") {
                if (!empty(dirStack))
                    dirStack.pop_back();

                continue;
            }

            dirStack.push_back(dir);
        }

        ostringstream oss;
        for (const auto dir : dirStack)
            oss << '/' << dir;

        const auto result = oss.str();
        return empty(result) ? "/" : result;
    }
};
