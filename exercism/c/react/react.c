#include "react.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#define GROW_ARRAY(data_type, size_type) \
    struct {                             \
        data_type *buf;                  \
        size_type len, capacity;         \
    }

#define GROW_ARRAY_INIT(ga) \
    do {                    \
        (ga)->buf = NULL;   \
        (ga)->len = 0;      \
        (ga)->capacity = 0; \
    } while (false)

#define GROW_ARRAY_PUSH(ga, value)                                           \
    do {                                                                     \
        if ((ga)->len >= (ga)->capacity) {                                   \
            (ga)->capacity = (ga)->capacity ? (ga)->capacity * 2 : 1;        \
            (ga)->buf = realloc(                                             \
                (ga)->buf, (ga)->capacity * sizeof *(ga)->buf); /* NOLINT */ \
            assert((ga)->buf);                                               \
        }                                                                    \
        (ga)->buf[(ga)->len++] = (value);                                    \
    } while (false)

enum cell_type {
    CELL_INPUT,
    CELL_COMPUTE1,
    CELL_COMPUTE2,
};

struct callback_data {
    callback cb;
    void *userdata;
};

struct cell {
    enum cell_type type;
    int value;
    union {
        compute1 compute1;
        compute2 compute2;
    } func;
    struct cell *c1, *c2;
    GROW_ARRAY(struct cell *, size_t) dep_cells;
    GROW_ARRAY(struct callback_data, callback_id) cbs;
};

struct reactor {
    GROW_ARRAY(struct cell *, size_t) cells;
};

struct reactor *create_reactor(void)
{
    struct reactor *const r = malloc(sizeof *r);
    assert(r);
    GROW_ARRAY_INIT(&r->cells);
    return r;
}

void destroy_reactor(struct reactor *const r)
{
    assert(r);
    for (size_t i = 0; i < r->cells.len; ++i) {
        free(r->cells.buf[i]->cbs.buf);
        free(r->cells.buf[i]->dep_cells.buf);
        free(r->cells.buf[i]);
    }
    free(r->cells.buf);
    free(r);
}

static inline struct cell *new_cell(struct reactor *const r,
                                    const enum cell_type t, const int value)
{
    assert(r);
    struct cell *const c = malloc(sizeof *c);
    assert(c);
    c->type = t;
    c->value = value;
    GROW_ARRAY_INIT(&c->dep_cells);
    GROW_ARRAY_INIT(&c->cbs);
    GROW_ARRAY_PUSH(&r->cells, c);
    return c;
}

struct cell *create_input_cell(struct reactor *const r, const int initial_value)
{
    return new_cell(r, CELL_INPUT, initial_value);
}

struct cell *create_compute1_cell(struct reactor *const r,
                                  struct cell *const c1, const compute1 func)
{
    assert(func && c1);
    struct cell *const c = new_cell(r, CELL_COMPUTE1, func(c1->value));
    c->func.compute1 = func;
    c->c1 = c1;
    GROW_ARRAY_PUSH(&c1->dep_cells, c);
    return c;
}

struct cell *create_compute2_cell(struct reactor *const r,
                                  struct cell *const restrict c1,
                                  struct cell *const restrict c2,
                                  const compute2 func)
{
    assert(func && c1 && c2);
    struct cell *const c =
        new_cell(r, CELL_COMPUTE2, func(c1->value, c2->value));
    c->func.compute2 = func;
    c->c1 = c1;
    c->c2 = c2;
    GROW_ARRAY_PUSH(&c1->dep_cells, c);
    GROW_ARRAY_PUSH(&c2->dep_cells, c);
    return c;
}

int get_cell_value(const struct cell *const c)
{
    assert(c);
    return c->value;
}

void set_cell_value(struct cell *const c, const int new_value)
{
    assert(c && c->type == CELL_INPUT);
    GROW_ARRAY(struct cell *, size_t) deps;
    GROW_ARRAY_INIT(&deps);
    GROW_ARRAY_PUSH(&deps, c);

    GROW_ARRAY(int, size_t) deps_old_vals;
    GROW_ARRAY_INIT(&deps_old_vals);

    for (size_t i = 0; i < deps.len; ++i) {
        struct cell *const depc = deps.buf[i];
        GROW_ARRAY_PUSH(&deps_old_vals, depc->value);

        switch (depc->type) {
        case CELL_INPUT:
            if (depc == c)
                depc->value = new_value;
            break;
        case CELL_COMPUTE1:
            depc->value = depc->func.compute1(depc->c1->value);
            break;
        case CELL_COMPUTE2:
            depc->value = depc->func.compute2(depc->c1->value, depc->c2->value);
            break;
        }

        if (depc->value == deps_old_vals.buf[deps_old_vals.len - 1])
            continue;

        for (size_t j = 0; j < depc->dep_cells.len; ++j)
            GROW_ARRAY_PUSH(&deps, depc->dep_cells.buf[j]);
    }

    for (size_t i = 0; i < deps.len; ++i) {
        const struct cell *const depc = deps.buf[i];
        size_t j = 0;
        for (; j < i && depc != deps.buf[j]
               && depc->value != deps_old_vals.buf[j];
             ++j) {}

        if (i != j || depc->value == deps_old_vals.buf[j])
            continue; // cb called already or value is same as starting value

        for (callback_id id = 0; id < depc->cbs.len; ++id) {
            if (depc->cbs.buf[id].cb)
                depc->cbs.buf[id].cb(depc->cbs.buf[id].userdata, depc->value);
        }
    }

    free(deps_old_vals.buf);
    free(deps.buf);
}

callback_id add_callback(struct cell *const c, void *const d, const callback cb)
{
    assert(c && cb);
    callback_id id = 0;
    for (; id < c->cbs.len && c->cbs.buf[id].cb; ++id) {}
    const struct callback_data cbd = {.cb = cb, .userdata = d};
    if (id < c->cbs.len)
        c->cbs.buf[id] = cbd;
    else
        GROW_ARRAY_PUSH(&c->cbs, cbd);

    return id;
}

void remove_callback(struct cell *const c, const callback_id id)
{
    assert(c && id < c->cbs.len);
    c->cbs.buf[id].cb = NULL;
}
