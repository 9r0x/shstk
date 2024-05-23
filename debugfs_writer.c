#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define READSSP(val) __asm__ volatile("rdsspq %0" : "=r"(val) : :);

int main()
{
    char device_path[256] = "/sys/kernel/debug/shstk_status/status";
    char buffer[4] = {0};
    int bytes_read;

    int fd = open(device_path, O_WRONLY);
    if (fd == -1)
    {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    unsigned long long ssp = 0;
    READSSP(ssp);
    printf("main: rdssp returned 0x%016llx\n", ssp);
    printf("value at ssp: 0x%016llx\n", *(unsigned long long *)ssp);
    *(unsigned long long *)buffer = ssp;

    bytes_read = write(fd, buffer, 4);
    if (bytes_read == -1)
    {
        perror("Error writing from file");
        close(fd);
        return EXIT_FAILURE;
    }

    close(fd);
    return EXIT_SUCCESS;
}
