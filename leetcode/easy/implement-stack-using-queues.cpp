// https://leetcode.com/problems/implement-stack-using-queues
// Complexity: runtime: init O(1), push O(n), pop O(1), top O(1), empty O(1),
//             space: O(n).

class MyStack {
    queue<int> vals_, tmp_;

public:
    MyStack() {}

    void push(const int x)
    {
        tmp_ = vals_;
        while (!::empty(vals_))
            vals_.pop();

        vals_.push(x);
        while (!::empty(tmp_)) {
            vals_.push(tmp_.front());
            tmp_.pop();
        }
    }

    int pop() noexcept
    {
        const auto val = top();
        vals_.pop();
        return val;
    }

    int top() const noexcept
    {
        return vals_.front();
    }

    bool empty() const noexcept
    {
        return ::empty(vals_);
    }
};

/**
 * Your MyStack object will be instantiated and called as such:
 * MyStack* obj = new MyStack();
 * obj->push(x);
 * int param_2 = obj->pop();
 * int param_3 = obj->top();
 * bool param_4 = obj->empty();
 */
