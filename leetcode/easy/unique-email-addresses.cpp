// https://leetcode.com/problems/unique-email-addresses
//
// 12ms runtime. (fast!)
// Complexity: runtime O(n*e), space O(n*e). [where n == size(emails), e ==
//                                            size(emails[i])]

class Solution {
public:
    int numUniqueEmails(const vector<string>& emails) const
    {
        unordered_set<string> uniques;

        for (const auto& e : emails) {
            auto atIdx = e.find_first_of('@');
            string_view user(e.c_str(), min(atIdx, e.find_first_of('+')));
            string_view domain(e.c_str() + atIdx, size(e) - atIdx);
            auto userDots = count(begin(user), end(user), '.');

            string unique;
            unique.reserve(size(user) + size(domain) - userDots);
            remove_copy(begin(user), end(user), back_inserter(unique), '.');

            unique.append(domain);
            uniques.insert(unique);
        }

        return size(uniques);
    }
};
