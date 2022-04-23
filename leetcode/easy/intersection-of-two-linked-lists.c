/* https://leetcode.com/problems/intersection-of-two-linked-lists */
/* Complexity: runtime O(a+b), space O(1). */

struct ListNode *getIntersectionNode(struct ListNode *const a,
                                     struct ListNode *const b)
{
    for (struct ListNode *n = a; n; n = n->next) {
        n->val *= -1;
    }

    struct ListNode *ret = NULL;
    for (struct ListNode *n = b; n; n = n->next) {
        if (n->val < 0) {
            ret = n;
            break;
        }
    }

    for (struct ListNode *n = a; n; n = n->next) {
        n->val *= -1;
    }
    return ret;
}
