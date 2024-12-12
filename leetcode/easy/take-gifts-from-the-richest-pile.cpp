// https://leetcode.com/problems/take-gifts-from-the-richest-pile
//
// Uses the low-level heap manipulation functions over priority_queue so we can
// accumulate() the answer in O(n).
//
// Complexity: runtime O(n+min(k,n)*logn), space O(1) [O(n) if counting input
// storage reused as heap].

class Solution {
public:
    long long pickGifts(vector<int> &gifts, int k) const
    {
        make_heap(gifts.begin(), gifts.end());
        for (; k > 0 && !gifts.empty(); --k) {
            const int next = sqrt(gifts.front());
            pop_heap(gifts.begin(), gifts.end());
            if (next <= 0) {
                gifts.pop_back();
                continue;
            }
            gifts.back() = next;
            push_heap(gifts.begin(), gifts.end());
        }
        return accumulate(gifts.begin(), gifts.end(), 0ll);
    }
};
