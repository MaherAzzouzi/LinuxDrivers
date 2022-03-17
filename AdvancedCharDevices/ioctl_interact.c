#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

#define MODULE_IOC_MAGIC 'M'
#define MODULE_READ		_IOR(MODULE_IOC_MAGIC, 0, char *)
#define MODULE_WRITE	_IOW(MODULE_IOC_MAGIC, 1, char *)

int main(void)
{
	char buf[0x1000] = {0};
	char buf2[0x1000] = {0};

	strcpy(buf, "This message will be forwarded to the kernel and returned \
	back");
	int fd = open("/dev/module0_ioctl", O_RDWR);
	ioctl(fd, MODULE_READ, buf);
	ioctl(fd, MODULE_WRITE, buf2);
	printf("Message from our shitty driver: %s\n", buf2);
}