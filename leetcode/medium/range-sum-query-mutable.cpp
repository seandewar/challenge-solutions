// https://leetcode.com/problems/range-sum-query-mutable/
//
// Decomposes the input into buckets of size ceil(sqrt(n)), storing the sum of
// the numbers in each bucket (known as sqrt decomposition). This allows us to
// calculate the sums by only traversing at most the sqrt(n) buckets.
//
// Complexity: lookup runtime O(sqrt(n)), pre-processing runtime O(n),
//             update runtime O(1), space O(sqrt(n)).

class NumArray {
    struct SumBucket {
        vector<int> nums;
        int sum;
    };

    vector<SumBucket> buckets_;
    const decltype(SumBucket::nums)::size_type bucketMaxSize_;

public:
    NumArray(vector<int>& nums)
        : bucketMaxSize_(ceil(sqrt(size(nums))))
    {
        SumBucket bucket{};

        for (const int x : nums) {
            if (size(bucket.nums) == bucketMaxSize_) {
                buckets_.push_back(move(bucket));

                bucket.~SumBucket();
                new (&bucket) SumBucket();
            }

            bucket.nums.push_back(x);
            bucket.sum += x;
        }

        if (!empty(bucket.nums))
            buckets_.push_back(move(bucket));
    }

    void update(int i, int val)
    {
        auto& bucket = buckets_[i / bucketMaxSize_];
        int& bucketNum = bucket.nums[i % bucketMaxSize_];

        bucket.sum += val - bucketNum;
        bucketNum = val;
    }

    int sumRange(int i, int j)
    {
        const auto beginIdx = i / bucketMaxSize_;
        const auto lastIdx = j / bucketMaxSize_;

        const auto beginNumsIdx = i % bucketMaxSize_;
        const auto lastNumsIdx = j % bucketMaxSize_;

        if (beginIdx == lastIdx) {
            const auto& bucketNums = buckets_[beginIdx].nums;
            return accumulate(cbegin(bucketNums) + beginNumsIdx,
                              cbegin(bucketNums) + lastNumsIdx + 1,
                              0);
        }

        int sum = 0;

        {
            const auto& beginNums = buckets_[beginIdx].nums;
            sum += accumulate(cbegin(beginNums) + beginNumsIdx,
                              cend(beginNums),
                              0);
        }

        {
            const auto& lastNums = buckets_[lastIdx].nums;
            sum += accumulate(cbegin(lastNums),
                              cbegin(lastNums) + lastNumsIdx + 1,
                              0);
        }

        sum += accumulate(cbegin(buckets_) + beginIdx + 1,
                          cbegin(buckets_) + lastIdx,
                          0,
                          [](const int sum, const auto& bucket) noexcept {
                              return sum + bucket.sum;
                          });

        return sum;
    }
};

/**
 * Your NumArray object will be instantiated and called as such:
 * NumArray* obj = new NumArray(nums);
 * obj->update(i,val);
 * int param_2 = obj->sumRange(i,j);
 */

// Alternative Solution:
//
// Complexity: lookup runtime O(1), constructor runtime O(n),
//             update runtime O(n), space O(n).
//
// class NumArray {
//     vector<int> accumSums_;
//
// public:
//     NumArray(vector<int>& nums)
//     {
//         accumSums_.reserve(size(nums) + 1);
//         accumSums_.push_back(0);
//         partial_sum(cbegin(nums), cend(nums), back_inserter(accumSums_));
//     }
//
//     void update(int i, int val)
//     {
//         const int oldVal = accumSums_[i + 1] - accumSums_[i];
//         for (++i; i < size(accumSums_); ++i) {
//             accumSums_[i] += val - oldVal;
//         }
//     }
//
//     int sumRange(int i, int j)
//     {
//         return accumSums_[j + 1] - accumSums_[i];
//     }
// };
