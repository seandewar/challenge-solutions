// https://leetcode.com/problems/design-twitter

struct Tweet {
    int id, tick;
    struct Tweet *next;
};

struct Follow {
    int id;
    UT_hash_handle hh;
};

struct User {
    int id;
    struct Tweet *tweets;
    struct Follow *follows;
    UT_hash_handle hh;
};

typedef struct {
    struct User *users;
    int next_tweet_tick;
} Twitter;

Twitter *twitterCreate(void)
{
    Twitter *t = malloc(sizeof *t);
    t->users = NULL;
    t->next_tweet_tick = 0;
    return t;
}

static struct User *getUser(Twitter *const t, const int id, const bool create)
{
    struct User *u;
    HASH_FIND_INT(t->users, &id, u);
    if (!u && create) {
        u = malloc(sizeof *u);
        u->id = id;
        u->tweets = NULL;
        u->follows = NULL;
        HASH_ADD_INT(t->users, id, u);
    }
    return u;
}

void twitterPostTweet(Twitter *const obj, const int userId, const int tweetId)
{
    struct User *const u = getUser(obj, userId, true);
    struct Tweet *const t = malloc(sizeof *t);
    t->id = tweetId;
    t->tick = obj->next_tweet_tick++;
    t->next = u->tweets;
    u->tweets = t;
}

int *twitterGetNewsFeed(const Twitter *const obj, const int userId,
                        int *const retSize)
{
    *retSize = 0;
    const struct User *const u = getUser(obj, userId, false);
    if (!u)
        return NULL;

    const int num_heads = 1 + HASH_COUNT(u->follows);
    const struct Tweet **const heads = malloc(num_heads * sizeof heads[0]);
    heads[0] = u->tweets;
    int i = 1;
    const struct Follow *f, *ftmp;
    HASH_ITER(hh, u->follows, f, ftmp) {
        const struct User *const fu = getUser(obj, f->id, false);
        heads[i++] = fu ? fu->tweets : NULL;
    }

    int *const ret = malloc(10 * sizeof ret[0]);
    while (*retSize < 10) {
        const struct Tweet **newest = &heads[0];
        for (i = 1; i < num_heads; ++i) {
            if (!*newest || (heads[i] && (*newest)->tick < heads[i]->tick))
                newest = &heads[i];
        }
        if (!*newest)
            break;

        ret[(*retSize)++] = (*newest)->id;
        *newest = (*newest)->next;
    }

    free(heads);
    return ret;
}

void twitterFollow(Twitter *const obj, const int followerId,
                   const int followeeId)
{
    struct User *const u = getUser(obj, followerId, true);
    struct Follow *f;
    HASH_FIND_INT(u->follows, &followeeId, f);
    if (f)
        return;

    f = malloc(sizeof *f);
    f->id = followeeId;
    HASH_ADD_INT(u->follows, id, f);
}

void twitterUnfollow(Twitter *const obj, const int followerId,
                     const int followeeId)
{
    struct User *const u = getUser(obj, followerId, false);
    if (!u)
        return;

    struct Follow *f;
    HASH_FIND_INT(u->follows, &followeeId, f);
    if (!f)
        return;

    HASH_DEL(u->follows, f);
    free(f);
}

void twitterFree(Twitter *const obj)
{
    struct User *u, *utmp;
    HASH_ITER(hh, obj->users, u, utmp) {
        HASH_DEL(obj->users, u);

        struct Follow *f, *ftmp;
        HASH_ITER(hh, u->follows, f, ftmp) {
            HASH_DEL(u->follows, f);
            free(f);
        }

        for (struct Tweet *t = u->tweets; t;) {
            struct Tweet *next = t->next;
            free(t);
            t = next;
        }
        free(u);
    }
}
