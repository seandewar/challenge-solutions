// https://leetcode.com/problems/design-a-number-container-system
// Complexity: runtime change O(logn), find O(1), space O(n).

class NumberContainers {
    unordered_map<int, int> index_to_num;
    unordered_map<int, set<int>> num_to_indices;

public:
    void change(const int index, const int number)
    {
        const auto it = index_to_num.find(index);
        if (it != index_to_num.end()) {
            if (it->second == number)
                return;

            const auto removed = num_to_indices.at(it->second).erase(index);
            assert(removed == 1);
            it->second = number;
        } else {
            index_to_num.emplace(index, number);
        }

        num_to_indices[number].insert(index);
    }

    int find(const int number) const
    {
        const auto it = num_to_indices.find(number);
        if (it == num_to_indices.end() || it->second.empty())
            return -1;

        return *it->second.begin();
    }
};
