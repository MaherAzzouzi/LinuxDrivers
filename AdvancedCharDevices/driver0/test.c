#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdio.h>

int main()
{
	off_t sz = 0;

	int fd = open("test.c", O_RDWR);
	printf("fd = %d\n", fd);
	int ret = ioctl(fd, FIOQSIZE, &sz);
	printf("The size = %ld\n", sz);
}