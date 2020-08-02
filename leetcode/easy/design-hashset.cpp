// https://leetcode.com/problems/design-hashset/
//
// Bitset solution.
// Complexity: runtime O(1), space O(1) [state size is constant].

class MyHashSet {
    bitset<1000001> state_;

public:
    /** Initialize your data structure here. */
    MyHashSet() = default;

    void add(const int key) { state_[key] = true; }
    void remove(const int key) { state_[key] = false; }

    /** Returns true if this set contains the specified element */
    bool contains(const int key) const { return state_[key]; }
};

/**
 * Your MyHashSet object will be instantiated and called as such:
 * MyHashSet* obj = new MyHashSet();
 * obj->add(key);
 * obj->remove(key);
 * bool param_3 = obj->contains(key);
 */

// Alternative Solution: Manual bit manipulation.
// Complexity: runtime O(1), space O(1) [state size is constant].

class MyHashSet {
    // CHAR_BIT equals numeric_limits<unsigned char>::digits(), but who wants to
    // type the latter all the time?
    array<byte, (1000001 / CHAR_BIT) + 1> state_{};

public:
    /** Initialize your data structure here. */
    MyHashSet() = default;

    void add(const int key)
    {
        state_[key / CHAR_BIT] |= static_cast<byte>(1 << (key % CHAR_BIT));
    }

    void remove(const int key)
    {
        state_[key / CHAR_BIT] &= ~static_cast<byte>(1 << (key % CHAR_BIT));
    }

    /** Returns true if this set contains the specified element */
    bool contains(const int key) const
    {
        return static_cast<bool>(state_[key / CHAR_BIT]
                                 & static_cast<byte>(1 << (key % CHAR_BIT)));
    }
};
