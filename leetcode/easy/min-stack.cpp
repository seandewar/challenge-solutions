// https://leetcode.com/problems/min-stack/
//
// Complexity: get minimum element runtime O(1), space O(n).

class MinStack {
    stack<pair<int, int>> stack_;

public:
    MinStack() {}

    void push(int x)
    {
        const int prevMin = empty(stack_) ? numeric_limits<int>::max()
                                          : stack_.top().second;
        stack_.emplace(x, min(x, prevMin));
    }

    void pop()
    {
        stack_.pop();
    }

    int top()
    {
        return stack_.top().first;
    }

    int getMin()
    {
        return stack_.top().second;
    }
};

/**
 * Your MinStack object will be instantiated and called as such:
 * MinStack* obj = new MinStack();
 * obj->push(x);
 * obj->pop();
 * int param_3 = obj->top();
 * int param_4 = obj->getMin();
 */
