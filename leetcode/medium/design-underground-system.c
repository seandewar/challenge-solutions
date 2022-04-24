/* https://leetcode.com/problems/design-underground-system */
/* Complexity: same as the Rust version. */

struct CustomerEntry {
    int id;
    const char *from;
    int start_time;
    UT_hash_handle hh;
};

#define KEY_SIZE 22
struct AvgTimeEntry {
    char key[KEY_SIZE];
    double time;
    int n;
    UT_hash_handle hh;
};

typedef struct {
    struct CustomerEntry *customers;
    struct AvgTimeEntry *avg_times;
} UndergroundSystem;

UndergroundSystem *undergroundSystemCreate(void)
{
    UndergroundSystem *const ret = malloc(sizeof *ret);
    *ret = (UndergroundSystem){
        .customers = NULL,
        .avg_times = NULL,
    };
    return ret;
}

void undergroundSystemCheckIn(UndergroundSystem *const obj, const int id,
                              const char *const stationName, const int t)
{
    struct CustomerEntry *const customer = malloc(sizeof *customer);
    *customer = (struct CustomerEntry){
        .id = id,
        .from = stationName,
        .start_time = t,
    };
    HASH_ADD_INT(obj->customers, id, customer);
}

#define DEFINE_KEY(name, from, to) \
    char name[KEY_SIZE];           \
    snprintf(name, KEY_SIZE, "%s,%s", from, to)

void undergroundSystemCheckOut(UndergroundSystem *const obj, const int id,
                               const char *const stationName, const int t)
{
    struct CustomerEntry *customer;
    HASH_FIND_INT(obj->customers, &id, customer);
    const double duration = t - customer->start_time;

    DEFINE_KEY(key, customer->from, stationName);
    struct AvgTimeEntry *avg_time;
    HASH_FIND_STR(obj->avg_times, key, avg_time);
    if (!avg_time) {
        avg_time = malloc(sizeof *avg_time);
        *avg_time = (struct AvgTimeEntry){
            .time = duration,
            .n = 1,
        };
        memcpy(avg_time->key, key, KEY_SIZE);
        HASH_ADD_STR(obj->avg_times, key, avg_time);
    } else {
        avg_time->time *= avg_time->n;
        avg_time->time /= ++avg_time->n;
        avg_time->time += duration / avg_time->n;
    }

    HASH_DEL(obj->customers, customer);
    free(customer);
}

double undergroundSystemGetAverageTime(const UndergroundSystem *const obj,
                                       const char *const startStation,
                                       const char *const endStation)
{
    DEFINE_KEY(key, startStation, endStation);
    const struct AvgTimeEntry *avg_time;
    HASH_FIND_STR(obj->avg_times, key, avg_time);
    return avg_time->time;
}

void undergroundSystemFree(UndergroundSystem *const obj)
{
    struct CustomerEntry *customer, *tmp_customer;
    HASH_ITER(hh, obj->customers, customer, tmp_customer) {
        HASH_DEL(obj->customers, customer);
        free(customer);
    }

    struct AvgTimeEntry *avg_time, *tmp_avg_time;
    HASH_ITER(hh, obj->avg_times, avg_time, tmp_avg_time) {
        HASH_DEL(obj->avg_times, avg_time);
        free(avg_time);
    }

    free(obj);
}

/* Alternative Solution: Same, but uses an extra hash table. */
/* Complexity: same. */

struct CustomerEntry {
    int id;
    const char *from;
    int start_time;
    UT_hash_handle hh;
};

struct AvgTimeEntry {
    const char *to;
    double time;
    int n;
    UT_hash_handle hh;
};

struct StationEntry {
    const char *from;
    struct AvgTimeEntry *entries;
    UT_hash_handle hh;
};

typedef struct {
    struct CustomerEntry *customers;
    struct StationEntry *stations;
} UndergroundSystem;

UndergroundSystem *undergroundSystemCreate(void)
{
    UndergroundSystem *const ret = malloc(sizeof *ret);
    *ret = (UndergroundSystem){
        .customers = NULL,
        .stations = NULL,
    };
    return ret;
}

void undergroundSystemCheckIn(UndergroundSystem *const obj, const int id,
                              const char *const stationName, const int t)
{
    struct CustomerEntry *const customer = malloc(sizeof *customer);
    *customer = (struct CustomerEntry){
        .id = id,
        .from = stationName,
        .start_time = t,
    };
    HASH_ADD_INT(obj->customers, id, customer);
}

void undergroundSystemCheckOut(UndergroundSystem *const obj, const int id,
                               const char *const stationName, const int t)
{
    struct CustomerEntry *customer;
    HASH_FIND_INT(obj->customers, &id, customer);
    const double duration = t - customer->start_time;

    struct StationEntry *station;
    HASH_FIND_STR(obj->stations, customer->from, station);
    if (!station) {
        station = malloc(sizeof *station);
        *station = (struct StationEntry){
            .from = customer->from,
            .entries = NULL,
        };
        HASH_ADD_KEYPTR(hh, obj->stations, station->from, strlen(station->from),
                        station);
    }

    struct AvgTimeEntry *avg_time;
    HASH_FIND_STR(station->entries, stationName, avg_time);
    if (!avg_time) {
        avg_time = malloc(sizeof *avg_time);
        *avg_time = (struct AvgTimeEntry){
            .to = stationName,
            .time = duration,
            .n = 1,
        };
        HASH_ADD_KEYPTR(hh, station->entries, avg_time->to,
                        strlen(avg_time->to), avg_time);
    } else {
        avg_time->time *= avg_time->n;
        avg_time->time /= ++avg_time->n;
        avg_time->time += duration / avg_time->n;
    }

    HASH_DEL(obj->customers, customer);
    free(customer);
}

double undergroundSystemGetAverageTime(const UndergroundSystem *const obj,
                                       const char *const startStation,
                                       const char *const endStation)
{
    const struct StationEntry *station;
    HASH_FIND_STR(obj->stations, startStation, station);

    const struct AvgTimeEntry *avg_time;
    HASH_FIND_STR(station->entries, endStation, avg_time);
    return avg_time->time;
}

void undergroundSystemFree(UndergroundSystem *const obj)
{
    struct CustomerEntry *customer, *tmp_customer;
    HASH_ITER(hh, obj->customers, customer, tmp_customer) {
        HASH_DEL(obj->customers, customer);
        free(customer);
    }

    struct StationEntry *station, *tmp_station;
    HASH_ITER(hh, obj->stations, station, tmp_station) {
        struct AvgTimeEntry *avg_time, *tmp_avg_time;
        HASH_ITER(hh, station->entries, avg_time, tmp_avg_time) {
            HASH_DEL(station->entries, avg_time);
            free(avg_time);
        }

        HASH_DEL(obj->stations, station);
        free(station);
    }

    free(obj);
}

/**
 * Your UndergroundSystem struct will be instantiated and called as such:
 * UndergroundSystem* obj = undergroundSystemCreate();
 * undergroundSystemCheckIn(obj, id, stationName, t);

 * undergroundSystemCheckOut(obj, id, stationName, t);

 * double param_3 = undergroundSystemGetAverageTime(obj, startStation,
 endStation);

 * undergroundSystemFree(obj);
*/
