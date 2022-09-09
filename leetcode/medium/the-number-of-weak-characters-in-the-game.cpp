// https://leetcode.com/problems/the-number-of-weak-characters-in-the-game
// Took me a while to see this solution (relies on smarter sorting than the
// heap-based solution).
// Complexity: runtime O(n*logn), space O(logn).

class Solution {
public:
  int numberOfWeakCharacters(vector<vector<int>> &properties) {
    sort(begin(properties), end(properties), [](auto &a, auto &b) {
      return a[0] > b[0] || (a[0] == b[0] && a[1] < b[1]);
    });
    int max1 = numeric_limits<int>::min(), ret = 0;
    for (auto& prop: properties) {
      max1 = max(max1, prop[1]);
      if (prop[1] < max1) {
        ++ret;
      }
    }
    return ret;
  }
};

// Alternative Solution: my original heap-based approach
// Complexity: runtime O(n*logn), space O(n).

class Solution {
public:
  int numberOfWeakCharacters(vector<vector<int>> &properties) {
    sort(begin(properties), end(properties), [](auto &a, auto &b) {
      return a[0] < b[0] || (a[0] == b[0] && a[1] < b[1]);
    });

    auto cmp0 = [](auto &a, auto &b) { return a[0] < b[0]; };
    vector<int> maxs;
    maxs.reserve(size(properties));
    priority_queue<int> pq;
    for (auto it = upper_bound(begin(properties), end(properties),
                               properties[0], cmp0);;) {
      it = upper_bound(it, end(properties), *it, cmp0);
      maxs.push_back((*(it - 1))[1]);
      pq.push(maxs.back());
      if (it == end(properties)) {
        break;
      }
    }
    if (empty(pq)) {
      return 0;
    }

    int maxi = 0, ret = 0;
    priority_queue<int> pqPop;
    for (auto it = begin(properties); it != end(properties); ++it) {
      if (it != begin(properties) && (*(it - 1))[0] != (*it)[0]) {
        const int pop = maxs[maxi++];
        if (pq.top() == pop) {
          pq.pop();
          while (!empty(pqPop) && pq.top() == pqPop.top()) {
            pq.pop();
            pqPop.pop();
          }
          if (empty(pq)) {
            break;
          }
        } else {
          pqPop.push(pop);
        }
      }
      if ((*it)[1] < pq.top()) {
        ++ret;
      }
    }
    return ret;
  }
};
