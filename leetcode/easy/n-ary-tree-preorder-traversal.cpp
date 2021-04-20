// https://leetcode.com/problems/n-ary-tree-preorder-traversal
//
// This solution updates the next childIdx in-place, rather than pop()ping and
// emplace()ing all the time.
//
// Complexity: runtime O(n), space O(height).

class Solution {
public:
    vector<int> preorder(const Node* root) const
    {
        vector<int> res;
        stack<pair<const Node*, size_t>> st;
        st.emplace(root, 0);

        while (!empty(st)) {
            auto& [node, childIdx] = st.top();
            if (node && childIdx == 0)
                res.push_back(node->val);
            if (!node || childIdx == size(node->children))
                st.pop();
            else
                st.emplace(node->children[childIdx++], 0);
        }

        return res;
    }
};
