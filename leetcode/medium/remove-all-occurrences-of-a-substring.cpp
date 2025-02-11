// https://leetcode.com/problems/remove-all-occurrences-of-a-substring
//
// This approach represents deleted ranges as intervals, merging intersecting or
// adjacent ranges together. The resulting string is constructed afterwards.
// Why did I go through all of this effort? Idk; it was fun.
//
// Complexity: runtime O(n*(m+logn)), space O(n).

class Solution {
public:
    string removeOccurrences(const string &s, const string &part) const
    {
        map<int, int> deleted_ranges;
        const auto next_char = [&](int &i, auto &deleted_after_it) {
            ++i;
            if (deleted_after_it != deleted_ranges.end()
                && i == deleted_after_it->first)
                ++deleted_after_it;
        };
        const auto skip_deleted = [&](int &i, const auto deleted_after_it) {
            if (deleted_after_it != deleted_ranges.begin()) {
                const auto [j, n] = *prev(deleted_after_it);
                if (i < j + n) {
                    i = j + n; // skip the intersecting deleted range
                    return true;
                }
            }
            return false;
        };

        auto deleted_after_i_it = deleted_ranges.end();
        for (int i = 0; i + part.size() <= s.size();) {
            if (skip_deleted(i, deleted_after_i_it))
                continue;
            if (s[i] != part[0]) {
                next_char(i, deleted_after_i_it);
                continue;
            }

            int j = i, part_i = 1;
            auto deleted_after_j_it = deleted_after_i_it;
            next_char(j, deleted_after_j_it);
            while (part_i < part.size() && j < s.size()) {
                if (skip_deleted(j, deleted_after_j_it))
                    continue;
                if (s[j] != part[part_i])
                    break;

                next_char(j, deleted_after_j_it);
                ++part_i;
            }
            if (part_i < part.size()) {
                next_char(i, deleted_after_i_it);
                continue;
            }

            // delete intersecting ranges
            const auto left_it = deleted_after_i_it != deleted_ranges.begin()
                                     ? prev(deleted_after_i_it)
                                     : deleted_ranges.end();
            const auto end_it = deleted_ranges.lower_bound(j);
            int n = j - i;
            for (auto it = deleted_after_i_it; it != end_it;)
                it = deleted_ranges.erase(it);

            // consume the range to the right if adjacent
            if (end_it != deleted_ranges.end()) {
                const auto [r, rn] = *end_it;
                if (j == r) {
                    n += rn;
                    deleted_ranges.erase(end_it);
                }
            }

            // deleting may cause chars to the left to become a prefix of part,
            // so go left part.size() - 1 chars after a deletion
            const auto restart_part = [&](auto deleted_it) {
                int di = deleted_it->first, dn = deleted_it->second;
                i = di + dn;
                for (int m = 0; m + 1 < part.size() && i > 0; ++m) {
                    if (--i < di + dn) {
                        i = max(0, di - 1);
                        if (deleted_it != deleted_ranges.begin()) {
                            --deleted_it;
                            di = deleted_it->first;
                            dn = deleted_it->second;
                        } else {
                            di = dn = 0;
                        }
                    }
                }
                deleted_after_i_it =
                    deleted_it->first <= i ? next(deleted_it) : deleted_it;
            };

            // merge with the range to the left if adjacent
            if (left_it != deleted_ranges.end()) {
                const auto [l, ln] = *left_it;
                if (l + ln == i) {
                    left_it->second += n;
                    restart_part(left_it);
                    continue;
                }
            }
            // not merging; insert the new range
            restart_part(deleted_ranges.emplace(i, n).first);
        }

        string ret;
        ret.reserve(ranges::fold_left(
            deleted_ranges, s.size(),
            [](const int acc, const auto &r) { return acc - r.second; }));

        int l = 0;
        for (const auto [i, n] : deleted_ranges) {
            ret.append(s, l, i - l);
            l = i + n;
        }
        ret.append(s, l);
        return ret;
    }
};
