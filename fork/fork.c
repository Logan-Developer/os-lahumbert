#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid;

    // Fork a child process
    pid = fork();

    if (pid < 0) {
        // Fork failed
        fprintf(stderr, "Fork failed\n");
        return 1;
    } else if (pid == 0) {
        // Child process
        printf("Child process created. PID: %d, PPID: %d\n", getpid(), getppid());
        printf("Child process working before exec\n");
        sleep(1);
        printf("Done\n");

        // Execute a different program
        execl("/bin/bash", "bash", "-c", "echo $$", NULL);

        // If exec fails, it will reach here
        fprintf(stderr, "Exec failed\n");
        return 1;
    } else {
        // Parent process
        printf("Parent process. PID: %d\n", getpid());
        printf("Child process PID: %d\n", pid);

        int status;

        // Wait for the child to finish
        wait(&status);

        if (WIFEXITED(status)) {
            printf("Child exited with status: %d\n", WEXITSTATUS(status));
        } else {
            printf("Child exited abnormally\n");
        }
    }

    return 0;
}
