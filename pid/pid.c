#include <stdio.h>
#include <unistd.h>

int main() {
    // getpid() returns the process ID of the current process
    pid_t pid = getpid();
    
    printf("The process ID (PID) is: %d\n", pid);

    return 0;
}
