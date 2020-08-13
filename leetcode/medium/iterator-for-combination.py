# https://leetcode.com/problems/iterator-for-combination/
#
# All the solutions here are lazily-evaluated.
#
# Complexity: init runtime O(k), next() runtime O(k), hasNext() runtime O(1),
# space O(k) [where n = len(characters), k = len(combinationLength)].

class CombinationIterator:
    def __init__(self, characters: str, combinationLength: int):
        self.characters = characters
        self.indices = list(reversed(range(combinationLength)))
        self.indices[0] -= 1 # so the first next() gets the first combination

    def next(self) -> str:
        for i in range(len(self.indices)):
            if self.indices[i] < len(self.characters) - 1 - i:
                self.indices[i] += 1
                for j in reversed(range(i)):
                    self.indices[j] = self.indices[j + 1] + 1
                break

        return ''.join([self.characters[i] for i in reversed(self.indices)])

    def hasNext(self) -> bool:
        return self.indices[-1] != len(self.characters) - len(self.indices)


# Your CombinationIterator object will be instantiated and called as such:
# obj = CombinationIterator(characters, combinationLength)
# param_1 = obj.next()
# param_2 = obj.hasNext()

# Alternative Solution: Using the itertools module.
#
# Complexity: init runtime O(k), next() runtime O(k), hasNext() runtime O(1),
# space O(k) [where n = len(characters), k = len(combinationLength)].

class CombinationIterator:
    def __init__(self, characters: str, combinationLength: int):
        self.nextCombIt = combinations(characters, combinationLength)
        self.nextComb = next(self.nextCombIt, None)

    def next(self) -> str:
        nextComb = self.nextComb
        self.nextComb = next(self.nextCombIt, None)
        return ''.join(nextComb)

    def hasNext(self) -> bool:
        return self.nextComb is not None

# Alternative Solution: Lazily-evaluated bit manipulation.
#
# Can be done more efficiently if you want, but still faster than ~73%
# submissions.
#
# Complexity: init runtime O(n), next() runtime O(n), hasNext() runtime O(k),
# space O(n) [where n = len(characters), k = len(combinationLength)].

class CombinationIterator:
    def __init__(self, characters: str, combinationLength: int):
        self.characters = characters
        self.combLen = combinationLength
        self.combMask = 2 ** len(characters)

    def next(self) -> str:
        self.combMask -= 1 # so we don't repeat the same combination
        while bin(self.combMask).count('1') != self.combLen:
            self.combMask -= 1

        result = []
        combMask = self.combMask

        for c in reversed(self.characters):
            if combMask & 1 == 1:
                result.append(c)
            combMask >>= 1

        return ''.join(reversed(result))

    def hasNext(self) -> bool:
        return (2 ** self.combLen - 1) & self.combMask != self.combMask
