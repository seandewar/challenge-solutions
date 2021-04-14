// https://leetcode.com/problems/flatten-nested-list-iterator
//
// Complexity: runtime O(TODO), space O(TODO).

/**
 * // This is the interface that allows for creating nested lists.
 * // You should not implement it, or speculate about its implementation
 * class NestedInteger {
 *   public:
 *     // Return true if this NestedInteger holds a single integer, rather than
 * a nested list. bool isInteger() const;
 *
 *     // Return the single integer that this NestedInteger holds, if it holds a
 * single integer
 *     // The result is undefined if this NestedInteger holds a nested list
 *     int getInteger() const;
 *
 *     // Return the nested list that this NestedInteger holds, if it holds a
 * nested list
 *     // The result is undefined if this NestedInteger holds a single integer
 *     const vector<NestedInteger> &getList() const;
 * };
 */

class NestedIterator {
    const vector<NestedInteger>& nestedList_;
    stack<pair<const vector<NestedInteger>*, size_t>> stack_;
    int next_;

    void updateNext()
    {
        if (empty(stack_)) {
            next_ = numeric_limits<int>::min();
            stack_.emplace(&nestedList_, 0);

            while (!stack_.top().first->front().isInteger()
                   && !stack_.top().first->front().getList().empty())
                stack_.emplace(&stack_.top().first->front().getList(), 0);
        } else
            ++stack_.top().second;

        while (!empty(stack_)) {
            if (!stack_.top().first
                || stack_.top().first->size() <= stack_.top().second) {
                stack_.pop();
                if (!empty(stack_))
                    ++stack_.top().second;

                continue;
            }
            if (!(*stack_.top().first)[stack_.top().second].isInteger()) {
                stack_.emplace(
                    &(*stack_.top().first)[stack_.top().second].getList(), 0);
                continue;
            }

            break;
        }

        if (empty(stack_))
            next_ = numeric_limits<int>::min();
        else
            next_ = (*stack_.top().first)[stack_.top().second].getInteger();
    }

public:
    NestedIterator(const vector<NestedInteger>& nestedList)
        : nestedList_(nestedList)
    {
        updateNext();
    }

    int next()
    {
        const int next = next_;
        updateNext();
        return next;
    }

    bool hasNext() { return next_ != numeric_limits<int>::min(); }
};
