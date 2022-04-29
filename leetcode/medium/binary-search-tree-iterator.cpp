// https://leetcode.com/problems/binary-search-tree-iterator
// Complexity: runtime: ctor O(logn) if balanced, O(n) otherwise,
//                      next() O(logn) if balanced, O(n) otherwise,
//                      hasNext() O(1),
//             space: O(logn) if balanced, O(n) otherwise.

class BSTIterator {
    stack<const TreeNode *> st_;

public:
    BSTIterator(const TreeNode *root)
    {
        for (; root; root = root->left)
            st_.push(root);
    }

    int next()
    {
        const auto popped = st_.top();
        st_.pop();
        if (popped->right) {
            st_.push(popped->right);
            while (st_.top()->left)
                st_.push(st_.top()->left);
        }

        return popped->val;
    }

    bool hasNext() const noexcept
    {
        return !empty(st_);
    }
};

/**
 * Your BSTIterator object will be instantiated and called as such:
 * BSTIterator* obj = new BSTIterator(root);
 * int param_1 = obj->next();
 * bool param_2 = obj->hasNext();
 */
