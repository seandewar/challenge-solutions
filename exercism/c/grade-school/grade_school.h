#ifndef GRADE_SCHOOL_H
#define GRADE_SCHOOL_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define MAX_NAME_LENGTH 20
#define MAX_STUDENTS 20

typedef struct {
    uint8_t grade;
    char name[MAX_NAME_LENGTH];
} student_t;

typedef struct {
    size_t count;
    student_t students[MAX_STUDENTS];
} roster_t;

void init_roster(roster_t *);
bool add_student(roster_t *, const char *, uint8_t);
roster_t get_grade(const roster_t *, uint8_t);

#endif
