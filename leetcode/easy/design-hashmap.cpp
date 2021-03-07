// https://leetcode.com/problems/design-hashmap
//
// Allocate an array of the maximum possible size for the question.
//
// In general, we should use the heap for such an array, but the stack is more
// than likely large enough for a million ints.
//
// Complexity: runtime O(1) all operations, space O(1).

class MyHashMap {
    array<int, 1'000'001> buf_;

public:
    MyHashMap() noexcept { fill(begin(buf_), end(buf_), -1); }
    void put(const int key, const int value) noexcept { buf_[key] = value; }
    [[nodiscard]] int get(const int key) const noexcept { return buf_[key]; }
    void remove(const int key) noexcept { put(key, -1); }
};

// Alternative Solution: Use buckets.
//
// Most hash maps are implemented using this idea, though another idea is to use
// an open addressing-based approach with linear/pseudo-random probing; that
// approach is good for memory-constrained environments, but because we know
// what the max amount of elements in our map will be, there's no need to go
// through implementing that approach (it entails dynamic resizing), so we'll
// use the bucket-based idea:
//
// We allocate a number of buckets, and calculate a bucket index from the hash
// code of the value's key we wish to access. Usually the bucket count starts
// low and increases according to some heuristic (requiring resizes after some
// number of inserts similar to a vector), but because we know the max number of
// values that will be inserted for this problem, we can just choose a number
// that's probably good enough(TM) (in general, we'd also use the heap for the
// array of buckets too).
//
// A power of 2 bucket count is ideal here, as we can calculate a bucket index
// by just performing (hash(k) & (n - 1)), rather than a modulo (any such modulo
// here might get optimized by the compiler to such a computation if n is a
// power of two anyway).
//
// From this bucket index, we insert the value with it's key along with the
// key's hash inside the bucket; this may be implemented as a bucket containing
// a resizeable array or a bucket containing a linked list. The linked list
// approach is usually used, even though vectors are usually more efficient to
// traverse due to their contiguous and cache-local storage, even when removing
// elements not at the end (although removal via swapping with the end and
// popping may be a more efficient option, which is used for this solution);
// this is because linked lists use slightly less storage when empty, a vector
// requires elements to be copy/moveable when a resize is required (also
// invalidates pointers), and the amortized cost of a vector insert due to
// possible resizes may be unacceptable for some applications. For simplicity,
// we'll use a vector here.
//
// If the bucket isn't empty, we'll have to compare the hash of our key with the
// hashes of the other keys to check that this key isn't already used. If two
// hashes are the same, we'll also need to compare both keys for equality to
// ensure this isn't due to a hash collision (two different keys producing the
// same hash). For this problem, the key can be used as the hash, so we only
// need to store the key and value.
//
// Complexity: runtime O(1) average, O(n) worst-case for all operations, space
// O(n), but *technically* O(1) due to the problem constraints.

class MyHashMap {
    array<vector<pair<int, int>>, 2 << 11> buckets_;

    auto& bucket(const int key) noexcept
    {
        return buckets_[key & (size(buckets_) - 1)];
    }

    const auto& bucket(const int key) const noexcept
    {
        return buckets_[key & (size(buckets_) - 1)];
    }

public:
    MyHashMap() noexcept = default;

    void put(const int key, const int value)
    {
        // collisions aren't an issue for this problem, as each key can be used
        // as a hash that will always be completely unique for a key; same
        // applies to the other methods
        auto& b = bucket(key);
        auto it = find_if(begin(b), end(b),
                          [key](const auto& kv) { return kv.first == key; });

        if (it != cend(b))
            it->second = value;
        else
            b.emplace_back(key, value);
    }

    [[nodiscard]] int get(const int key) const noexcept
    {
        const auto& b = bucket(key);
        auto it = find_if(cbegin(b), cend(b),
                          [key](const auto& kv) { return kv.first == key; });

        return it != cend(b) ? it->second : -1;
    }

    void remove(const int key)
    {
        auto& b = bucket(key);
        b.erase(remove_if(begin(b), end(b),
                          [key](const auto& kv) { return kv.first == key; }),
                cend(b));
    }
};
