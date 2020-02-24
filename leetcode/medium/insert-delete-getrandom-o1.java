// https://leetcode.com/problems/insert-delete-getrandom-o1/
//
// Uses an array list to store the set's values and a hash map to map each
// stored value to its index within the array list.
//
// To maintain an average O(1) time for removal, we maintain a logical size of
// the set (from dataIndexMap.size()), which may differ from the actual size of
// the set's array list used to store its data. Removal simply involves
// overwriting the to-be removed value with the one at the logical end of the
// set, decreasing the logical size of the set afterwards by removing the to-be
// removed value's entry from within the index map (decrements
// dataIndexMap.size()).
//
// See code comments for details.
//
// Complexity: runtime: average O(1) for all operations, space O(n).

class RandomizedSet {
    private final List<Integer> data = new ArrayList<>();
    private final Map<Integer, Integer> dataIndexMap = new HashMap<>();
    private final Random rng = new Random();

    /** Initialize your data structure here. */
    public RandomizedSet() {}

    /** Inserts a value to the set. Returns true if the set did not already
     * contain the specified element. */
    public boolean insert(final int val) {
        if (!dataIndexMap.containsKey(val)) {
            // if the logical size of our set is less than the actual size of
            // the data container backing it, we may simply overwrite the junk
            // data at the next index past our logical end (set()). otherwise,
            // we need to expand the size of the container (add())

            if (dataIndexMap.size() < data.size())
                data.set(dataIndexMap.size(), val); // push to logical end
            else
                data.add(val); // push to container end

            dataIndexMap.put(val, dataIndexMap.size());
            return true;
        }

        return false;
    }

    /** Removes a value from the set. Returns true if the set contained the
     * specified element. */
    public boolean remove(final int val) {
        final var i = dataIndexMap.get(val);

        if (i != null) {
            // to remove in O(1) time, we don't actually modify the size of our
            // data container. instead, we decrease the logical size of our set
            // by replacing the to-be removed val with the one at the logical
            // end of the data, then decrease the data's logical size by
            // removing the to-be removed val's entry within our index mappings

            final var logicalEndVal = data.get(dataIndexMap.size() - 1);
            data.set(i, logicalEndVal);
            dataIndexMap.replace(logicalEndVal, i);

            dataIndexMap.remove(val);
            return true;
        }

        return false;
    }

    /** Get a random element from the set. */
    public int getRandom() {
        // choose a random val in our data from the index [0, logical size)
        return data.get(rng.nextInt(dataIndexMap.size()));
    }
}

/**
 * Your RandomizedSet object will be instantiated and called as such:
 * RandomizedSet obj = new RandomizedSet();
 * boolean param_1 = obj.insert(val);
 * boolean param_2 = obj.remove(val);
 * int param_3 = obj.getRandom();
 */
