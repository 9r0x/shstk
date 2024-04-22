#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int main()
{
    char device_path[256] = "/sys/kernel/debug/shstk_status/status";
    char buffer[BUFFER_SIZE];
    int bytes_read;

    int fd = open(device_path, O_RDONLY);
    if (fd == -1)
    {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    bytes_read = read(fd, buffer, BUFFER_SIZE);
    if (bytes_read == -1)
    {
        perror("Error reading from file");
        close(fd);
        return EXIT_FAILURE;
    }

    close(fd);
    return EXIT_SUCCESS;
}
