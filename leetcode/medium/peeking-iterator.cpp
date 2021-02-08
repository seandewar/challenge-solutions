// https://leetcode.com/problems/peeking-iterator
//
// Minimal example using sentinel value.
// Technically, we shouldn't use a sentinel, as the constraints for the vals in
// nums isn't stated by the question, but oh well.
//
// Complexity: runtime O(1) all operations, space O(1).

/*
 * Below is the interface for Iterator, which is already defined for you.
 * **DO NOT** modify the interface for Iterator.
 *
 *  class Iterator {
 *		struct Data;
 * 		Data* data;
 *		Iterator(const vector<int>& nums);
 * 		Iterator(const Iterator& iter);
 *
 * 		// Returns the next element in the iteration.
 *		int next();
 *
 *		// Returns true if the iteration has more elements.
 *		bool hasNext() const;
 *	};
 */

class PeekingIterator : public Iterator {
    static const int NoNext = numeric_limits<int>::min();
    int next_ = NoNext;

public:
    PeekingIterator(const vector<int>& nums) : Iterator(nums)
    {
        // Initialize any member here.
        // **DO NOT** save a copy of nums and manipulate it directly.
        // You should only use the Iterator interface methods.
        next();
    }

    // Returns the next element in the iteration without advancing the iterator.
    [[nodiscard]] auto peek() const noexcept -> int { return next_; }

    // hasNext() and next() should behave the same as in the Iterator interface.
    // Override them if needed.
    auto next() noexcept -> int
    {
        return exchange(next_, Iterator::hasNext() ? Iterator::next() : NoNext);
    }

    [[nodiscard]] auto hasNext() const noexcept -> bool
    {
        return next_ != NoNext;
    }
};

// Alternative Solution: Using std::optional and modern C++ API design :P
// Complexity: runtime O(1) all operations, space O(1).

class PeekingIterator : public Iterator {
    optional<int> next_;

public:
    PeekingIterator(const vector<int>& nums) noexcept : Iterator(nums)
    {
        if (Iterator::hasNext())
            next_.emplace(Iterator::next());
    }

    [[nodiscard]] auto peek() const noexcept -> int { return *next_; }

    // Not [[nodiscard]], as we may just want to advance the iterator without
    // using the next value for anything.
    auto next() -> int
    {
        if (!hasNext())
            throw runtime_error("no next value");

        return *exchange(next_, Iterator::hasNext()
                                    ? optional<int>(Iterator::next())
                                    : nullopt);
    }

    [[nodiscard]] auto hasNext() const noexcept -> bool
    {
        // We can just return next_; here, as std::optional implicitly converts
        // to bool denoting the value of has_next() (similiar to pointers and
        // nullptr), but it's nice to be explicit here.
        return next_.has_value();
    }
};
