#include "grade_school.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

void init_roster(roster_t *r)
{
    r->count = 0;
}

static int cmp_students(const void *a, const void *b)
{
    const student_t *l = a, *r = b;

    return l->grade != r->grade ? (int)l->grade - r->grade
                                : strcmp(l->name, r->name);
}

bool add_student(roster_t *r, const char *name, uint8_t grade)
{
    const size_t name_len = strlen(name);
    if (r->count >= MAX_STUDENTS || name_len + 1 > MAX_NAME_LENGTH) {
        return false;
    }

    // We could bsearch(), but MAX_STUDENTS is small enough anyway.
    for (size_t i = 0; i < r->count; ++i) {
        if (strcmp(r->students[i].name, name) == 0) {
            return false;
        }
    }

    // strcpy() also works, but we already know the length.
    memcpy(r->students[r->count].name, name, name_len + 1);
    r->students[r->count].grade = grade;
    ++r->count;

    // We could simply insert the student in the right place (and move the
    // students after one position up), but qsort() is more convenient.
    qsort(r->students, r->count, sizeof r->students[0], cmp_students);
    return true;
}

roster_t get_grade(const roster_t *r, uint8_t grade)
{
    roster_t result;
    init_roster(&result);

    for (size_t i = 0; i < r->count; ++i) {
        const student_t *s = &r->students[i];

        if (s->grade == grade) {
            const bool ok = add_student(&result, s->name, s->grade);
            assert(ok);
        }
    }

    return result;
}
