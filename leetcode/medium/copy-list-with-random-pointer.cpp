// https://leetcode.com/problems/copy-list-with-random-pointer
//
// Hash Table Solution.
// It's possible to do this without the hash map, but I'm tight on time today.
// :(
//
// Complexity: runtime O(n), space O(n).

/*
// Definition for a Node.
class Node {
public:
    int val;
    Node* next;
    Node* random;

    Node(int _val) {
        val = _val;
        next = NULL;
        random = NULL;
    }
};
*/

class Solution {
public:
    auto copyRandomList(const Node* head) const -> Node*
    {
        unordered_map<const Node*, Node*> oldToNewMap;
        oldToNewMap.emplace(nullptr, nullptr);
        Node fakeHead(0);

        for (auto newEnd = &fakeHead; head; head = head->next) {
            const auto newNode = new Node(*head);
            newEnd = newEnd->next = newNode;
            oldToNewMap[head] = newNode;
        }

        for (auto node = fakeHead.next; node; node = node->next)
            node->random = oldToNewMap.at(node->random);

        return fakeHead.next;
    }
};
