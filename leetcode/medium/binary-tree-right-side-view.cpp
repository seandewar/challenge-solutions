// https://leetcode.com/problems/binary-tree-right-side-view/
//
// Complexity: runtime O(n), space O(n).

/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
class Solution {
public:
    vector<int> rightSideView(TreeNode* root)
    {
        if (!root) {
            return {};
        }

        vector<int> results;
        queue<TreeNode*> q{{root}};

        while (!empty(queue)) {
            const auto size = size(queue);

            for (decltype(queue)::size_type i = 0; i < size; ++i) {
                const auto frontNode = queue.front();
                queue.pop();

                if (i == 0) {
                    results.push_back(frontNode->val);
                }
                if (frontNode->right) {
                    queue.push(frontNode->right);
                }
                if (frontNode->left) {
                    queue.push(frontNode->left);
                }
            }
        }

        return results;
    }
};
