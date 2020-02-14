// https://leetcode.com/problems/student-attendance-record-i/
//
// Complexity: runtime O(n).

class Solution {
public:
    bool checkRecord(const string& s) const
    {
        return count(cbegin(s), cend(s), 'A') < 2
               && s.find("LLL") == string::npos;
    }
};

// Alternative Solution: using regular expressions.
// Complexity: ideally runtime O(n).
/*
class Solution {
public:
    bool checkRecord(const string& s) const
    {
        // or !regex_match(s, regex(".*A.*A.*|.*LLL.*")), but regex_search will
        // terminate without needing to match the whole string
        return !regex_search(s, regex("A.*A|LLL"));
    }
};
*/

// Alternative Solution: manual looping and counting.
// Complexity: runtime O(n), space O(1).
/*
class Solution {
public:
    bool checkRecord(const string& s) const
    {
        int absentCount = 0, lateStreak = 0;

        for (const char c : s) {
            if (c == 'L') {
                if (++lateStreak > 2)
                    return false;
            } else {
                lateStreak = 0;

                if (c == 'A' && ++absentCount > 1)
                    return false;
            }
        }

        return true;
    }
};
*/
