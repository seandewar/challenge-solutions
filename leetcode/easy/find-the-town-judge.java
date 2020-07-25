// https://leetcode.com/problems/find-the-town-judge/
//
// Complexity: runtime O(n), space O(n).

class Solution {
    public int findJudge(final int N, final int[][] trust) {
        final var trustedBy = Stream.generate(ArrayList<Integer>::new).limit(N)
                .collect(Collectors.toCollection(ArrayList::new));
        final var trustsNobodySet = IntStream.rangeClosed(1, N).boxed().collect(
                Collectors.toCollection(HashSet::new));

        for (final var pt : trust) {
            trustedBy.get(pt[1] - 1).add(pt[0]);
            trustsNobodySet.remove(pt[0]);
        }

        int judge = -1;
        for (final var p : trustsNobodySet) {
            // everybody (except the town judge) trusts the town judge. input
            // will not contain duplicates or invalid trust relationships, so we
            // can test for this condition by simply testing the size of the set
            // of people trusting this person
            if (trustedBy.get(p - 1).size() == N - 1) {
                // if a judge was already found, then there are at least two
                // judges, which fails to satisfy problem requirement 3 (there
                // should be exactly one judge)
                if (judge != -1)
                    return -1;

                judge = p;
            }
        }

        return judge;
    }
}
