// https://leetcode.com/problems/happy-number/
//
// Using hash set.
// Complexity: runtime O(logn), space O(logn).

class Solution {
public:
    bool isHappy(int n) const
    {
        unordered_set<int> seen;

        while (n != 1) {
            if (seen.count(n) > 0)
                return false; // endless loop if we've seen this sum before

            seen.insert(n);
            int next = 0;

            for (; n > 0; n /= 10)
                next += (n % 10) * (n % 10);

            n = next;
        }

        return true;
    }
};

// Alternative Solution: using Floyd's cycle detection algorithm. See my
// solution for "Linked List Cycle II" (medium) for more details.
//
// Complexity: runtime O(logn), space O(1).
/*
class Solution {
    int computeHappySum(int n) const
    {
        int result = 0;

        for (; n > 0; n /= 10)
            result += (n % 10) * (n % 10);

        return result;
    }

public:
    bool isHappy(const int n) const
    {
        int fast = n, slow = n;

        while ((fast = computeHappySum(computeHappySum(fast))) != 1) {
            slow = computeHappySum(slow);

            if (fast == slow)
                return false;
        }

        return true;
    }
};
*/
