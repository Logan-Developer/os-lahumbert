#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>

void check(int checkState) {
    if (checkState > 0) {
        check(checkState-1);
    }
    printf("Checking state %d\n", checkState);
}

int main() {
    // getpid() returns the process ID of the current process
    pid_t pid = getpid();

    uint32_t size = 4096;
    uint8_t *buffer = (uint8_t*) malloc((sizeof(uint8_t) * size));

    if (buffer == NULL) {
        printf("Failed to allocate memory\n");
        return 1;
    }
    
    printf("The process ID (PID) is: %d\n", pid);

    free(buffer);

    check(10);

    return 0;
}
