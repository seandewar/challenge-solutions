// https://leetcode.com/problems/kth-missing-positive-number/
//
// Logarithmic-time solution using binary search.
// Complexity: runtime O(logn), space O(1).

class Solution {
public:
    int findKthPositive(const vector<int>& arr, const int k) const noexcept
    {
        size_t left = 0, right = size(arr);

        while (left < right) {
            const size_t i = left + (right - left) / 2;
            const int numMissing = arr[i] - (i + 1);

            if (numMissing >= k)
                right = i;
            else
                left = i + 1;
        }

        return left + k;
    }
};

// Alternative Solution: Simplified linear-time solution.
// Complexity: runtime O(n), space O(1).

class Solution {
public:
    int findKthPositive(const vector<int>& arr, const int k) const noexcept
    {
        size_t i = 0;

        for (; i < size(arr); ++i) {
            if (const int numMissing = arr[i] - (i + 1); numMissing >= k)
                break;
        }

        return i + k;
    }
};

// Alternative Solution: Linear-time solution.
// Complexity: runtime O(n), space O(1).

class Solution {
public:
    int findKthPositive(const vector<int>& arr, const int k) const noexcept
    {
        for (size_t i = 0; i < size(arr); ++i) {
            if (const int numMissing = arr[i] - (i + 1); numMissing >= k)
                return arr[i] - (numMissing - k + 1); // same as i + k
        }

        // missing number outside the array
        const int numMissing = arr.back() - size(arr); // at the last element
        return arr.back() + (k - numMissing);
    }
};
