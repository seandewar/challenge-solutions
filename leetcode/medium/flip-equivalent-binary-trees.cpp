// https://leetcode.com/problems/flip-equivalent-binary-trees
// Complexity: runtime O(n), space O(d).

class Solution {
public:
    bool flipEquiv(const TreeNode *const t1, const TreeNode *const t2) const
    {
        if (!t1 || !t2)
            return !t1 && !t2;
        if (t1->val != t2->val)
            return false;

        return (flipEquiv(t1->left, t2->right)
                && flipEquiv(t1->right, t2->left))
               || (flipEquiv(t1->left, t2->left)
                   && flipEquiv(t1->right, t2->right));
    }
};
