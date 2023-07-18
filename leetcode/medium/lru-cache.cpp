// https://leetcode.com/problems/lru-cache
// Complexity: runtime amortized O(1), space O(n) (technically O(10,000) == O(1)
// due to constraints :P).

class LRUCache {
  list<pair<int, int>> kvs_;
  array<optional<decltype(kvs_)::iterator>, 10'000> kmap_{};
  int cap_;

public:
  LRUCache(int capacity) : cap_(capacity) {}

  int get(int key) {
    if (!kmap_[key])
      return -1;
    int value = (*kmap_[key])->second;
    kvs_.erase(*kmap_[key]);
    kmap_[key] = kvs_.emplace(end(kvs_), key, value);
    return value;
  }

  void put(int key, int value) {
    if (kmap_[key])
      kvs_.erase(*kmap_[key]);
    kmap_[key] = kvs_.emplace(end(kvs_), key, value);
    if (size(kvs_) > cap_) {
      kmap_[kvs_.front().first] = nullopt;
      kvs_.pop_front();
    }
  }
};
