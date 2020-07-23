// https://leetcode.com/problems/course-schedule-ii/
//
// DFS + in-degree counting solution.
//
// Builds a dependency graph from the courses (using a map vector), traversing
// the graph starting from courses without any prerequisites (represented by
// nodes with in-degrees of 0). During traversal, the in-degree count for each
// node is decreased, appending the courses for nodes with a 0 in-degree count
// to the result (courses whose requirements are now met).
//
// If there are nodes with in-degree counts > 0 by the end of the algorithms
// execution, then there are courses whose requirements aren't possible to meet,
// indicating an impossible ordering. Likewise, if there is a cycle in the
// dependency graph, detectable by a node's in-degree count going negative, the
// ordering is impossible.
//
// Complexity: runtime O(n+e), space O(n+e) [where n = number of courses
// (nodes/vertices), e = number of edges (arcs/dependency relations)].

class Solution {
    bool dfsBuildOrder(const vector<vector<int>>& prereqToMap,
                       vector<int>& numInDegreesMap, vector<int>& out,
                       const int course) const
    {
        if (numInDegreesMap[course] <= 0) {
            return false; // course already taken; we're in a cycle
        }

        if (--numInDegreesMap[course] == 0) {
            out.push_back(course);

            for (const int c : prereqToMap[course]) {
                if (!dfsBuildOrder(prereqToMap, numInDegreesMap, out, c)) {
                    return false; // propagate failure
                }
            }
        }

        return true;
    }

public:
    vector<int> findOrder(const int numCourses,
                          const vector<vector<int>>& prereqs) const
    {
        vector<vector<int>> prereqToMap(numCourses);
        vector<int> numInDegreesMap(numCourses);

        for (const auto& r : prereqs) {
            prereqToMap[r[1]].push_back(r[0]);
            ++numInDegreesMap[r[0]];
        }

        vector<int> result, noPrereqs;
        result.reserve(numCourses);

        // courses without prereqs can be taken beforehand in any order
        for (size_t course = 0; course < size(numInDegreesMap); ++course) {
            if (numInDegreesMap[course] == 0) {
                noPrereqs.push_back(course);
                result.push_back(course);
            }
        }

        // start building the course order from the courses that require the
        // no-prereq courses as dependencies
        for (const int course : noPrereqs) {
            for (const int c : prereqToMap[course]) {
                if (!dfsBuildOrder(prereqToMap, numInDegreesMap, result, c)) {
                    return {}; // cycle detected; fail
                }
            }
        }

        // failed if we didn't manage to take all courses
        if (any_of(cbegin(numInDegreesMap), cend(numInDegreesMap),
                   [](const int n) { return n != 0; })) {
            return {};
        }

        return result;
    }
};
