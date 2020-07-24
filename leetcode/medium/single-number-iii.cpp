// https://leetcode.com/problems/single-number-iii/
//
// XOR constant-space solution. See code comments for details.
// Complexity: runtime O(n), space O(1).

class Solution {
public:
    vector<int> singleNumber(const vector<int>& nums) const
    {
        // xor all nums. because x^x=0, and because xor is commutative (a^b=b^a)
        // and associative ((a^b)^c=a^(b^c)), all nums that appear twice will
        // cancel out to 0, leaving us with the xor of the 2 nums that only
        // appear once
        auto partMask = 0u;

        for (const unsigned x : nums) {
            partMask ^= x;
        }

        // we now have to identify which 2 nums didn't cancel out, as these are
        // the 2 nums that appear only once. because each digit set in a xor
        // indicates a differing digit in the same position of both nums, we can
        // partition the nums into 2 sets: nums with and nums without a
        // particular bit set in the same position as some set bit within the
        // xor result from earlier. we'll partition using the right-most set bit
        // within our xor result as it's easy to do, but any set bit works.

        // the reason this works as to find the rightmost bit is as follows:
        // inverting the number causes all rightmost unset bits to be set, and
        // adding 1 causes those rightmost bits to all unset again due to the
        // cascading carry, eventually causing the original rightmost set bit to
        // become set again. because of the inversion and addition, that
        // original rightmost set bit that was set again will now be the only
        // bit that is the same after these operations; we can preserve only it
        // by performing a bitwise and with the original number
        partMask &= -partMask; // same as &= ~partMask + 1 due to 2s complement

        // now we xor the nums in the array with a different number depending on
        // whether or not they have the same bit set as our partition bit. this
        // will yield the 2 different nums that appear only once
        int result1 = 0, result2 = 0;

        for (const unsigned x : nums) {
            (x & partMask ? result1 : result2) ^= x;
        }

        return {result1, result2};
    }
};

// Alternative Solution: Hash map.
// Complexity: runtime O(n), space O(n).

class Solution {
public:
    vector<int> singleNumber(const vector<int>& nums) const
    {
        unordered_map<int, size_t> numCountMap;

        for (const int x : nums) {
            ++numCountMap[x];
        }

        // input guarantees exactly 2 numbers that appear once, so this
        // never fails
        constexpr auto pred = [](const auto& kv) { return kv.second == 1; };
        const auto fIt = find_if(cbegin(numCountMap), cend(numCountMap), pred);

        return {fIt->first, find_if(next(fIt), cend(numCountMap), pred)->first};
    }
};
