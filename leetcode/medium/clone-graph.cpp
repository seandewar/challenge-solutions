// https://leetcode.com/problems/clone-graph
// Complexity: runtime O(n), space O(n).

/*
// Definition for a Node.
class Node {
public:
    int val;
    vector<Node*> neighbors;
    Node() {
        val = 0;
        neighbors = vector<Node*>();
    }
    Node(int _val) {
        val = _val;
        neighbors = vector<Node*>();
    }
    Node(int _val, vector<Node*> _neighbors) {
        val = _val;
        neighbors = _neighbors;
    }
};
*/
class Solution {
    Node *dfs(Node *node, array<Node *, 100> &map)
    {
        auto &ret = map[node->val - 1];
        if (!ret) {
            ret = new Node(node->val);
            for (auto n : node->neighbors)
                ret->neighbors.push_back(dfs(n, map));
        }
        return ret;
    }

public:
    Node *cloneGraph(Node *node)
    {
        if (!node)
            return nullptr;
        array<Node *, 100> map{};
        return dfs(node, map);
    }
};
