// https://leetcode.com/problems/fruit-into-baskets
//
// Complexity: runtime O(n), space O(1).

class Solution {
public:
    int totalFruit(const vector<int>& tree) const
    {
        auto j = find_if(cbegin(tree) + 1, cend(tree),
                         [&](int x) { return x != tree[0]; });
        int result = j - cbegin(tree);

        for (auto i = cbegin(tree); j != cend(tree);) {
            auto cond = [&](int x) { return x != *i && x != *j; };
            j = find_if(j + 1, cend(tree), cond);
            result = max<int>(result, j - i);
            if (j == cend(tree))
                break;

            i = j - 1;
            i = find_if(reverse_iterator(i), crend(tree), cond).base();
            result = max<int>(result, j - i);
        }

        return result;
    }
};
