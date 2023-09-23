#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(const int argc, const char *const *const argv)
{
    for (int i = 1; i < argc; ++i) {
        const int fd = open(argv[i], O_RDONLY);
        if (fd == -1) {
            perror("open");
            return EXIT_FAILURE;
        }

        char buf[4096];
        for (ssize_t read_len; (read_len = read(fd, buf, sizeof buf)) != 0;) {
            if (read_len == -1) {
                perror("read");
                goto fail;
            }
            if (fwrite(buf, 1, read_len, stdout) != read_len) {
                perror("fwrite");
                goto fail;
            }
        }

        if (close(fd) == -1) {
            perror("close");
            return EXIT_FAILURE;
        }
        continue;

    fail:
        if (close(fd) == -1)
            perror("close");
        return EXIT_FAILURE;
    }
}
