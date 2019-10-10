// https://leetcode.com/problems/design-circular-queue/

class MyCircularQueue {
    unique_ptr<int[]> data_;
    int capacity_, size_, frontIdx_;

public:
    /** Initialize your data structure here. Set the size of the queue to be k. */
    MyCircularQueue(int k)
        : data_(new int[k]), capacity_(k), size_(0), frontIdx_(0) {}

    /** Insert an element into the circular queue. Return true if the operation is successful. */
    bool enQueue(int value)
    {
        if (isFull()) {
            return false;
        }

        data_[(frontIdx_ + size_++) % capacity_] = value;
        return true;
    }

    /** Delete an element from the circular queue. Return true if the operation is successful. */
    bool deQueue()
    {
        if (isEmpty()) {
            return false;
        }

        frontIdx_ = --size_ > 0 ? (frontIdx_ + 1) % capacity_ : 0;
        return true;
    }

    /** Get the front item from the queue. */
    int Front()
    {
        return !isEmpty() ? data_[frontIdx_] : -1;
    }

    /** Get the last item from the queue. */
    int Rear()
    {
        return !isEmpty() ? data_[(frontIdx_ + size_ - 1) % capacity_] : -1;
    }

    /** Checks whether the circular queue is empty or not. */
    bool isEmpty()
    {
        return size_ == 0;
    }

    /** Checks whether the circular queue is full or not. */
    bool isFull()
    {
        return size_ == capacity_;
    }
};

/**
 * Your MyCircularQueue object will be instantiated and called as such:
 * MyCircularQueue* obj = new MyCircularQueue(k);
 * bool param_1 = obj->enQueue(value);
 * bool param_2 = obj->deQueue();
 * int param_3 = obj->Front();
 * int param_4 = obj->Rear();
 * bool param_5 = obj->isEmpty();
 * bool param_6 = obj->isFull();
 */
