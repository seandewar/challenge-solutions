// https://leetcode.com/problems/top-k-frequent-elements/
//
// Using a hash map.
// Complexity: runtime O(k*n), space O(n).

class Solution {
public:
    vector<int> topKFrequent(const vector<int>& nums, int k) const
    {
        unordered_map<int, int> countMap;

        for (const int x : nums)
            ++countMap[x];

        vector<int> results;
        results.reserve(k);

        for (; k > 0; --k) {
            const auto nextMaxIt = max_element(cbegin(countMap), cend(countMap),
                [](const auto& a, const auto& b) {
                    return a.second < b.second;
                });

            results.push_back(nextMaxIt->first);
            countMap.erase(nextMaxIt);
        }

        return results;
    }
};

// Alternative Solution: using max heap and hash map.
// Complexity: runtime O(n+k*logn), space O(n).
/*
class Solution {
public:
    vector<int> topKFrequent(const vector<int>& nums, int k) const
    {
        unordered_map<int, int> countMap;

        for (const int x : nums)
            ++countMap[x];

        // construct a vector of the unique numbers in nums, we can do this by
        // making a vector from the keys in the countMap
        vector<int> uniqueNums;
        uniqueNums.reserve(size(countMap));

        for (const auto& kv : countMap)
            uniqueNums.push_back(kv.first);

        // construct a max heap from the uniqueNums, using their counts in
        // countMap as the sorting criteria - this takes linear time to do, but
        // subsequent pushes/pops take logarithmic time
        const auto compare = [&](const int a, const int b) {
                                 return countMap[a] < countMap[b];
                             };
        priority_queue<int, vector<int>, decltype(compare)>
            heap(compare, move(uniqueNums));

        vector<int> results;
        results.reserve(k);

        for (; k > 0; --k) {
            results.push_back(heap.top());
            heap.pop(); // this takes logarithmic time
        }

        return results;
    }
};
*/
