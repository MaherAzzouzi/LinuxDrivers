#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <error.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    char bytes[2] = {11, 0};

    if (argc == 2)
        bytes[1] = atoi(argv[1]);
    else
    {
        fprintf(stderr, "%s: need a single arg!\n", argv[0]);
    }
    if (ioctl(STDIN_FILENO, TIOCLINUX, bytes) < 0)
    {
        fprintf(stderr, "%s: ioclt(STDIN_FILENO, TIOCLINUX): %s\n", argv[0], strerror(errno));
        exit(1);
    }

    return 0;
}
