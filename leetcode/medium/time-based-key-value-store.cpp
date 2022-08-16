// https://leetcode.com/problems/time-based-key-value-store
// Complexity: runtime O(w*n) [w = string keys, n = worst case vector insert],
//             space O(n).

class TimeMap {
    struct Entry {
        int ts;
        string v;
    };
    unordered_map<string, vector<Entry>> store_;

public:
    TimeMap() = default;

    void set(const string &key, const string &value, const int timestamp)
    {
        auto &e = store_[key];
        const auto it = lower_bound(
            begin(e), end(e), Entry{timestamp},
            [](const auto &a, const auto &b) { return a.ts < b.ts; });
        if (it != end(e) && it->ts == timestamp) {
            it->v = value;
        } else {
            e.insert(it, Entry{timestamp, value});
        }
    }

    string get(const string &key, const int timestamp) const
    {
        const auto fit = store_.find(key);
        if (fit == end(store_)) {
            return "";
        }
        const auto &e = fit->second;
        const auto it = lower_bound(
            begin(e), end(e), Entry{timestamp},
            [](const auto &a, const auto &b) { return a.ts < b.ts; });
        if (it != end(e) && it->ts == timestamp) {
            return it->v;
        } else if (it != begin(e)) {
            return (it - 1)->v;
        }
        return "";
    }
};

/**
 * Your TimeMap object will be instantiated and called as such:
 * TimeMap* obj = new TimeMap();
 * obj->set(key,value,timestamp);
 * string param_2 = obj->get(key,timestamp);
 */
