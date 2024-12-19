// https://leetcode.com/problems/final-prices-with-a-special-discount-in-a-shop
// Complexity: runtime O(n), space O(n).

class Solution {
public:
    vector<int> finalPrices(vector<int> &prices) const
    {
        vector<int> st;
        for (int i = 0; i < prices.size(); ++i) {
            for (; !st.empty() && prices[st.back()] >= prices[i]; st.pop_back())
                prices[st.back()] -= prices[i];
            st.push_back(i);
        }
        return prices;
    }
};
