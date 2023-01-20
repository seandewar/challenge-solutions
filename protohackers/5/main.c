#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <inttypes.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/ip.h>
#include <poll.h>
#include <pthread.h>
#include <sys/socket.h>
#include <unistd.h>

#define ADDR_STRBUF_LEN (INET_ADDRSTRLEN + sizeof ":XXXXX")

static void addr_to_str(const struct sockaddr_in *const addr,
                        char s[static ADDR_STRBUF_LEN])
{
    const char *const sp =
        inet_ntop(AF_INET, &addr->sin_addr, s, ADDR_STRBUF_LEN);
    assert(sp != NULL);
    const size_t len = strlen(s);
    snprintf(s + len, ADDR_STRBUF_LEN - len, ":%" PRIu16,
             ntohs(addr->sin_port));
}

#define UPSTREAM_NAME "chat.protohackers.com"
#define UPSTREAM_PORT 16963
#define LISTEN_PORT 1234
#define MAX_CLIENTS 10

struct client {
    atomic_int sock;
    int upstream_sock;
    struct sockaddr_in addr;
    char addr_s[ADDR_STRBUF_LEN];
};

// I just let the OS close sockets and such when using this.
static void exit_on_err(const int ret, const char *const msg)
{
    if (ret == -1) {
        perror(msg);
        exit(EXIT_FAILURE);
    }
}

static void *handle_client(void *);

int main(void)
{
    const struct hostent *const upstream_hostent = gethostbyname(UPSTREAM_NAME);
    if (upstream_hostent == NULL) {
        herror("gethostbyname (upstream)");
        return EXIT_FAILURE;
    }

    struct sockaddr_in upstream_addr = {
        .sin_family = AF_INET,
        .sin_port = htons(UPSTREAM_PORT),
    };
    memcpy(&upstream_addr.sin_addr, upstream_hostent->h_addr,
           sizeof upstream_addr.sin_addr);
    char upstream_addr_s[ADDR_STRBUF_LEN];
    addr_to_str(&upstream_addr, upstream_addr_s);
    printf("Resolved upstream %s:%d to %s\n", UPSTREAM_NAME, UPSTREAM_PORT,
           upstream_addr_s);

    const int listen_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    exit_on_err(listen_sock, "socket (listen)");

    struct sockaddr_in listen_addr = {
        .sin_family = AF_INET,
        .sin_port = htons(LISTEN_PORT),
    };
    const uint32_t in_addr_any = htonl(INADDR_ANY);
    memcpy(&listen_addr.sin_addr, &in_addr_any, sizeof listen_addr.sin_addr);
    exit_on_err(bind(listen_sock, (const struct sockaddr *)&listen_addr,
                     sizeof listen_addr),
                "bind");
    exit_on_err(listen(listen_sock, MAX_CLIENTS), "listen");
    printf("Listening on port %d...\n", LISTEN_PORT);

    struct client clients[MAX_CLIENTS];
    for (size_t i = 0; i < MAX_CLIENTS; ++i) {
        clients[i].sock = -1; // Sentinel indicates unused slot.
    }

    while (true) {
        struct client client;
        socklen_t client_addrlen = sizeof client.addr;
        client.sock = accept(listen_sock, (struct sockaddr *)&client.addr,
                             &client_addrlen);
        if (client.sock == -1) {
            perror("accept");
            continue; // No need to goto fail; nothing to clean up.
        }
        client.upstream_sock = -1; // Sentinel is checked by goto fail.

        addr_to_str(&client.addr, client.addr_s);
        printf("%s: connected!\n", client.addr_s);

        size_t client_i = 0;
        for (; client_i < MAX_CLIENTS && clients[client_i].sock != -1;
             ++client_i) {} // Ye ol' linear scan.
        if (client_i == MAX_CLIENTS) {
            fprintf(stderr, "Dropping %s: no free slots\n", client.addr_s);
            goto fail;
        }

        client.upstream_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (client.upstream_sock == -1) {
            fprintf(stderr, "Dropping %s: upstream socket() failed: %s\n",
                    client.addr_s, strerror(errno));
            goto fail;
        }

        if (connect(client.upstream_sock,
                    (const struct sockaddr *)&upstream_addr,
                    sizeof upstream_addr)
            == -1) {
            fprintf(stderr, "Dropping %s: upstream connect() failed: %s\n",
                    client.addr_s, strerror(errno));
            goto fail;
        }

        struct sockaddr_in upstream_localaddr;
        socklen_t upstream_sockname_len = sizeof upstream_localaddr;
        exit_on_err(getsockname(client.upstream_sock,
                                (struct sockaddr *)&upstream_localaddr,
                                &upstream_sockname_len),
                    "getsockname");
        char upstream_localaddr_s[ADDR_STRBUF_LEN];
        addr_to_str(&upstream_localaddr, upstream_localaddr_s);
        printf("%s: connected to upstream using %s\n", client.addr_s,
               upstream_localaddr_s);
        clients[client_i] = client;

        pthread_t thread;
        const int result =
            pthread_create(&thread, NULL, handle_client, &clients[client_i]);
        if (result != 0) {
            fprintf(stderr, "Dropping %s: pthread_create() failed: %d\n",
                    client.addr_s, result);
            goto fail;
        }
        continue;

    fail:
        if (client.upstream_sock != -1) {
            exit_on_err(close(client.upstream_sock), "close");
        }
        exit_on_err(close(client.sock), "close");
        if (client_i != MAX_CLIENTS) {
            clients[client_i].sock = -1;
        }
    }
}

#define TONY_BOGUSCOIN_ADDR "7YWHMfk9JZe0LM0g1ZauHuiSxhI"

static const char *next_boguscoin_addr(const char *const line_start,
                                       const char *const search_start,
                                       const char **const search_end)
{
    for (const char *s = search_start; s != *search_end;) {
        s = memchr(s, '7', *search_end - s);
        if (s == NULL) {
            break;
        }

        const char *e = s + 1;
        // isalnum() is locale-dependant, but it's easy to use.
        for (; e != *search_end && e - s < 35 && isalnum(*e); ++e) {}
        if (e - s < 26 || (s != line_start && *(s - 1) != ' ')
            || (e != *search_end && *e != ' ')) {
            s = e;
            continue;
        }

        *search_end = e;
        return s;
    }
    return NULL;
}

#define READ_BUF_LEN 1024

static bool handle_bridge(const int src_sock, const int dst_sock,
                          char read_buf[static READ_BUF_LEN],
                          size_t *const read_buf_len, const char *const addr_s,
                          const char *const src_s)
{
    const size_t read_len =
        read(src_sock, read_buf + *read_buf_len, READ_BUF_LEN - *read_buf_len);
    if (read_len == 0) {
        printf("Dropping %s: %s disconnected\n", addr_s, src_s);
        return false;
    } else if (read_len == -1) {
        fprintf(stderr, "Dropping %s: %s read() failed: %s\n", addr_s, src_s,
                strerror(errno));
        return false;
    }
    *read_buf_len += read_len;

    char *line_start = read_buf;
    for (; line_start != read_buf + *read_buf_len;) {
        char *const check_start = line_start == read_buf
                                      ? read_buf + *read_buf_len - read_len
                                      : line_start;
        char *const line_end =
            memchr(check_start, '\n', *read_buf_len - (check_start - read_buf));
        if (line_end == NULL) {
            break;
        }

        char line_buf[2 * READ_BUF_LEN];
        char *line_buf_end = line_buf;

        char *search_start = line_start;
        while (search_start != line_end) {
            char *boguscoin_end = line_end;
            char *const boguscoin_start = (char *)next_boguscoin_addr(
                line_start, search_start, (const char **)&boguscoin_end);
            if (boguscoin_start == NULL) {
                // No more boguscoin addresses on this line.
                goto next_line;
            }

            memcpy(line_buf_end, search_start, boguscoin_start - search_start);
            line_buf_end += boguscoin_start - search_start;

            memcpy(line_buf_end, TONY_BOGUSCOIN_ADDR,
                   strlen(TONY_BOGUSCOIN_ADDR));
            line_buf_end += strlen(TONY_BOGUSCOIN_ADDR);

            search_start = boguscoin_end;
        }

    next_line:
        // + 1 to include the NL.
        memcpy(line_buf_end, search_start, line_end + 1 - search_start);
        line_buf_end += line_end + 1 - search_start;

        printf("%s: %s sends: \"", addr_s, src_s);
        for (const char *p = line_buf; p != line_buf_end; ++p) {
            if (*p == '\n') {
                printf("\\n");
            } else {
                putchar(isprint(*p) ? *p : '?');
            }
        }
        puts("\"");

        for (ssize_t sent_len = 0; sent_len != line_buf_end - line_buf;) {
            const ssize_t send_len =
                send(dst_sock, line_buf, line_buf_end - line_buf, 0);
            if (send_len == -1) {
                fprintf(stderr, "Dropping %s: %s send() failed: %s\n", addr_s,
                        src_s, strerror(errno));
                return false;
            }
            sent_len += send_len;
        }

        line_start = line_end + 1;
    }

    memmove(read_buf, line_start, *read_buf_len);
    *read_buf_len -= line_start - read_buf;
    if (*read_buf_len == READ_BUF_LEN) {
        fprintf(stderr, "Dropping %s: %s read_buf overflow\n", addr_s, src_s);
        return false;
    }

    return true;
}

static void *handle_client(void *const arg)
{
    struct client *const client = arg;
    struct pollfd poll_fds[2] = {{
                                     .fd = client->sock,
                                     .events = POLLIN,
                                 },
                                 {
                                     .fd = client->upstream_sock,
                                     .events = POLLIN,
                                 }};

    char read_buf[READ_BUF_LEN], read_upstream_buf[READ_BUF_LEN];
    size_t read_buf_len = 0, read_upstream_buf_len = 0;

    while (true) {
        if (poll(poll_fds, 2, -1) == -1) {
            fprintf(stderr, "Dropping %s: poll() failed: %s\n", client->addr_s,
                    strerror(errno));
            break;
        }

        // sock
        if (poll_fds[0].revents != 0
            && !handle_bridge(client->sock, client->upstream_sock, read_buf,
                              &read_buf_len, client->addr_s, "client")) {
            break;
        }

        // upstream_sock
        if (poll_fds[1].revents != 0
            && !handle_bridge(client->upstream_sock, client->sock,
                              read_upstream_buf, &read_upstream_buf_len,
                              client->addr_s, "upstream")) {
            break;
        }
    }

    exit_on_err(close(client->upstream_sock), "close");
    exit_on_err(close(client->sock), "close");
    client->sock = -1;
    return NULL;
}
