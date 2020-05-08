// https://leetcode.com/problems/check-if-it-is-a-straight-line/
//
// Calculate and check for cross product == 0 for slopes between points.
// See alternative solution below for more information regarding gradients.
//
// Complexity: runtime O(n), space O(1).

class Solution {
public:
    bool checkStraightLine(const vector<vector<int>>& coordinates) const
    {
        // guaranteed to have 2 points in input. we don't have to worry about
        // duplicate points either
        int expectedDy = coordinates[0][1] - coordinates[1][1];
        int expectedDx = coordinates[0][0] - coordinates[1][0];

        for (vector<vector<int>>::size_type i = 2; i < size(coordinates); ++i) {
            const int dy = coordinates[0][1] - coordinates[i][1];
            const int dx = coordinates[0][0] - coordinates[i][0];

            // cross product == 0 when vectors are co-linear or zero (which
            // isn't possible as there are no duplicate points in input).
            //
            // calculate the cross product between both gradient vectors
            // (equivalent to dy * expectedDx != dx * expectedDy)
            if (dy * expectedDx - dx * expectedDy != 0)
                return false;
        }

        return true;
    }
};

// Alternative Solution: Calculate and compare gradients.
//
// If all points are on the same line, the gradient between each point should be
// the same; we can simply check if the gradient between the 1st and 2nd point
// is the same as the gradients between the 1st and all other points.
//
// This method is also used in my solution for the "Max Points on a Line"
// leetcode problem (hard difficulty).
//
// Complexity: runtime Omega(n) lower bound; depends on the implementation of
// std::gcd() [maybe Euclid's algorithm?], space O(1).

class Solution {
public:
    bool checkStraightLine(const vector<vector<int>>& coordinates) const
    {
        // guaranteed to have 2 points in input. we don't have to worry about
        // duplicate points either
        int expectedDy = abs(coordinates[0][1] - coordinates[1][1]);
        int expectedDx = abs(coordinates[0][0] - coordinates[1][0]);

        {
            const int slopeGcd = gcd(expectedDy, expectedDx);
            expectedDy /= slopeGcd;
            expectedDx /= slopeGcd;
        }

        for (vector<vector<int>>::size_type i = 2; i < size(coordinates); ++i) {
            const int dy = abs(coordinates[0][1] - coordinates[i][1]);
            const int dx = abs(coordinates[0][0] - coordinates[i][0]);
            const int slopeGcd = gcd(dy, dx);

            if (expectedDy != dy / slopeGcd || expectedDx != dx / slopeGcd)
                return false;
        }

        return true;
    }
};
