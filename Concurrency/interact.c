#include <stdio.h>
#include <fcntl.h>
#include <pthread.h>
#include <unistd.h>
#include <stdint.h>

void* puter(void *arg)
{
    int fd = (uint64_t)arg;
    for(;;)
        write(fd, NULL, 0);
}

int main()
{
    pthread_t tid;

    uint64_t fd = open("/dev/sem_device", O_RDWR);
    pthread_create(&tid, NULL, puter, (void*)fd);
    sleep(2);
    for(;;)
        read(fd, NULL, 0);
    pthread_join(tid, NULL);
}