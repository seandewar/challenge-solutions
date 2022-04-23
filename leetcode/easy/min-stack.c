/* https://leetcode.com/problems/min-stack */
/* Complexity: runtime: create O(1), push amortized O(1), pop O(1), top O(1),
 *                      min O(1).
 *             space O(n). */

struct Entry {
    int val;
    int min;
};

typedef struct {
    struct Entry *buf;
    size_t len;
    size_t capacity;
} MinStack;

MinStack *minStackCreate(void)
{
    MinStack *const ret = malloc(sizeof(MinStack));
    *ret = (MinStack){
        .buf = NULL,
        .len = 0,
        .capacity = 0,
    };

    return ret;
}

void minStackPush(MinStack *const obj, const int val)
{
    if (obj->len == obj->capacity) {
        obj->capacity = obj->capacity ? obj->capacity * 2 : 1;
        obj->buf = realloc(obj->buf, (sizeof *obj->buf) * obj->capacity);
    }

    obj->buf[obj->len] = (struct Entry){
        .val = val,
        .min = obj->len == 0 || val < obj->buf[obj->len - 1].min
                   ? val
                   : obj->buf[obj->len - 1].min,
    };
    ++obj->len;
}

void minStackPop(MinStack *const obj)
{
    --obj->len;
}

int minStackTop(const MinStack *const obj)
{
    return obj->buf[obj->len - 1].val;
}

int minStackGetMin(const MinStack *const obj)
{
    return obj->buf[obj->len - 1].min;
}

void minStackFree(MinStack *const obj)
{
    free(obj->buf);
    free(obj);
}

/**
 * Your MinStack struct will be instantiated and called as such:
 * MinStack* obj = minStackCreate();
 * minStackPush(obj, val);

 * minStackPop(obj);

 * int param_3 = minStackTop(obj);

 * int param_4 = minStackGetMin(obj);

 * minStackFree(obj);
*/
