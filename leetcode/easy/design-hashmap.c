/* https://leetcode.com/problems/design-hashmap */
/* Simple hash map as an array of buckets containing linked lists. */
/* Complexity: runtime: typical amortized O(1), space O(n). */

typedef struct Node {
    struct Node *next;
    int key;
    int val;
} Node;

#define BUFSIZE (1 << 13)
typedef struct {
    Node *buf[BUFSIZE];
} MyHashMap;

MyHashMap *myHashMapCreate(void)
{
    return calloc(1, sizeof(MyHashMap)); // NULL is 0 on cool machines :)
}

static inline Node *get(MyHashMap *const obj, const int key, size_t *const outi,
                        Node **const outprev)
{
    *outi = key % BUFSIZE;
    Node *p = NULL, *n = obj->buf[*outi]; // best hash func ever
    for (; n && n->key != key; p = n, n = n->next);
    *outprev = p;
    return n;
}

void myHashMapPut(MyHashMap *const obj, const int key, const int val)
{
    size_t i;
    Node *p, *n = get(obj, key, &i, &p);
    if (!n) {
        n = malloc(sizeof(Node));
        *n = (Node){.next = obj->buf[i], .key = key};
        obj->buf[i] = n;
    }
    n->val = val;
}

int myHashMapGet(const MyHashMap *const obj, const int key)
{
    size_t i;
    Node *p, *const n = get((MyHashMap *)obj, key, &i, &p);
    return n ? n->val : -1;
}

void myHashMapRemove(MyHashMap *const obj, const int key)
{
    size_t i;
    Node *p, *const n = get(obj, key, &i, &p);
    if (n) {
        *(p ? &p->next : &obj->buf[i]) = n->next;
        free(n);
    }
}

void myHashMapFree(MyHashMap *const obj)
{
    for (size_t i = 0; i < BUFSIZE; ++i) {
        while (obj->buf[i]) {
            Node *const next = obj->buf[i]->next;
            free(obj->buf[i]);
            obj->buf[i] = next;
        }
    }
    free(obj);
}
