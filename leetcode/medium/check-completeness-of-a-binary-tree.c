/* https://leetcode.com/problems/check-completeness-of-a-binary-tree */
/*
 * Uses a queue implemented via a singly-linked list.
 * Surprisingly, this performs faster than 95% of C solutions...
 */
/* Complexity: runtime O(n), space O(n). */

struct QueueNode {
    struct TreeNode *node;
    struct QueueNode *next;
    int i;
};

bool isCompleteTree(struct TreeNode *const root)
{
    struct QueueNode *qfront = NULL, *qback = NULL;
    int qsize = 0;
#define QUEUE_PUSH(n, idx)                                               \
    do {                                                                 \
        struct QueueNode *const _n = malloc(sizeof(struct QueueNode));   \
        *_n = (struct QueueNode){.node = (n), .next = NULL, .i = (idx)}; \
        qback = *(qback ? &qback->next : &qfront) = _n;                  \
        ++qsize;                                                         \
    } while (false)

    bool ret = true;
    QUEUE_PUSH(root, 0);
    for (int d = 1, dmaxsize = 1; qfront; ++d, dmaxsize *= 2) {
        const int dsize = qsize;
        for (int i = 0; i < dsize; ++i) {
            const struct QueueNode n = *qfront;
            free(qfront);
            --qsize;
            if (!(qfront = n.next))
                qback = NULL;

            if (dsize != dmaxsize
                && (i != n.i || n.node->left || n.node->right)) {
                ret = false;
                goto end;
            }
            if (n.node->left)
                QUEUE_PUSH(n.node->left, 2 * i);
            if (n.node->right)
                QUEUE_PUSH(n.node->right, 2 * i + 1);
        }
    }

end:
    while (qfront) {
        struct QueueNode *const next = qfront->next;
        free(qfront);
        qfront = next;
    }
    return ret;
}
