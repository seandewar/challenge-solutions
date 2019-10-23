// https://www.hackerrank.com/challenges/ctci-linked-list-cycle

/*
Detect a cycle in a linked list. Note that the head pointer may be 'NULL' if the list is empty.

A Node is defined as:
    struct Node {
        int data;
        struct Node* next;
    }
*/

#include <unordered_set>

using namespace std;

bool has_cycle(Node* head)
{
    unordered_set<Node*> visited_nodes;

    auto n = head;
    while (n) {
        if (visited_nodes.find(n) != cend(visited_nodes))
            return true;

        visited_nodes.insert(n);
        n = n->next;
    }

    return false;
}
