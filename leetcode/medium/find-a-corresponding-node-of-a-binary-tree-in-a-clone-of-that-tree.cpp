// https://leetcode.com/problems/find-a-corresponding-node-of-a-binary-tree-in-a-clone-of-that-tree/
// Iterative solution.
// Complexity: runtime O(n), space O(n) [O(logn) if balanced tree].

class Solution {
public:
    TreeNode* getTargetCopy(TreeNode* const original, TreeNode* const cloned,
                            const TreeNode* const target) const
    {
        stack s(deque{pair(original, cloned)});

        while (!empty(s)) {
            const auto [o, c] = s.top();
            s.pop();

            if (!o || !c)
                continue;

            if (o == target)
                return c;

            s.emplace(o->left, c->left);
            s.emplace(o->right, c->right);
        }

        return nullptr;
    }
};

// Alternative Solution: Recursive.
// Complexity: same.

class Solution {
public:
  TreeNode *getTargetCopy(TreeNode *const original, TreeNode *const cloned,
                          const TreeNode *const target) const noexcept {
    if (!original)
      return nullptr;

    if (original == target)
      return cloned;

    if (const auto result = getTargetCopy(original->left, cloned->left, target))
      return result;

    return getTargetCopy(original->right, cloned->right, target);
  }
};
