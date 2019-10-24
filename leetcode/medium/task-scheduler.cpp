// https://leetcode.com/problems/task-scheduler/
//
// Complexity: runtime O(n), space O(1).

class Solution {
public:
    int leastInterval(vector<char>& tasks, int n)
    {
        array<size_t, 26> taskCounts{}; // map not needed; lookup table suffices
        for (const char t : tasks)
            ++taskCounts[t - 'A'];

        // loses the alphabetical ordering, but we don't need it anyway
        sort(begin(taskCounts), end(taskCounts), greater<size_t>());

        int result = 0;

        while (taskCounts.front() > 0) {
            size_t i = 0;

            // loop until we process all of each type of task once, or if our
            // task type with the highest quantity's cooldown expires (latter
            // point is required for the most efficient ordering)
            for(; i < size(taskCounts) && taskCounts[i] > 0; ++i) {

                // re-sort if the cooldown for the task with the highest
                // quantity ended before we tried doing the tasks with lesser
                // quantities, as the tasks types we managed to process may
                // have less remaining now than the ones we didn't get to do
                if (i > n) {
                    sort(begin(taskCounts), end(taskCounts), greater<size_t>());
                    break;
                }

                --taskCounts[i];
            }

            // if we've not finished all tasks and the highest quantity task's
            // cooldown hasn't expired yet, then we've had to use n+1 CPU time
            // (i+(n-i+1); +1 because the cooldown begins AFTER processing the
            // first task), otherwise, only the time taken to do the tasks has
            // elapsed (i)
            result += i <= n && taskCounts.front() > 0 ? n + 1 : i;
        }

        return result;
    }
};
