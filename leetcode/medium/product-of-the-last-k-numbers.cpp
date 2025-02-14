// https://leetcode.com/problems/product-of-the-last-k-numbers
// Complexity: runtime add O(1), getProduct O(1) each; O(n) total, space O(n).

class ProductOfNumbers {
    vector<long long> prods{1};

public:
    void add(const int num)
    {
        if (num != 0)
            prods.push_back(prods.back() * num);
        else
            prods = {1};
    }

    int getProduct(const int k) const
    {
        if (k >= prods.size())
            return 0;
        return prods.back() / prods[prods.size() - k - 1];
    }
};
