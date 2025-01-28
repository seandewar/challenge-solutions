// https://leetcode.com/problems/course-schedule-iv
// Complexity: runtime O(n^3+q), space O(n^2).

class Solution {
public:
    vector<bool> checkIfPrerequisite(const int numCourses,
                                     const vector<vector<int>> &prerequisites,
                                     const vector<vector<int>> &queries) const
    {
        vector<vector<int>> graph(numCourses);
        for (const auto &pre : prerequisites)
            graph[pre[0]].push_back(pre[1]);

        vector<bitset<100>> prereqSet(numCourses);
        bitset<100> currSet;
        const auto fillPrereqSet = [&](this const auto &self,
                                       const int course) -> void {
            if (currSet.any() && (prereqSet[course] & currSet) == currSet)
                return;

            prereqSet[course] |= currSet;
            currSet[course] = true;

            for (const int otherCourse : graph[course])
                self(otherCourse);

            currSet[course] = false; // back-track
        };
        for (int i = 0; i < numCourses; ++i)
            fillPrereqSet(i);

        vector<bool> ret;
        ret.reserve(queries.size());
        for (const auto &q : queries)
            ret.push_back(prereqSet[q[1]][q[0]]);

        return ret;
    }
};
