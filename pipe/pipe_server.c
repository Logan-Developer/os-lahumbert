#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define FIFO1 "client_to_server.pipe"
#define FIFO2 "server_to_client.pipe"

int main()
{
    char buffer[1024];
    int read_fd, write_fd;

    // Create the named pipes (if they don't exist)
    mkfifo(FIFO1, 0666);
    mkfifo(FIFO2, 0666);

    // Open the pipes
    read_fd = open(FIFO1, O_RDONLY);  // Open client-to-server pipe for reading
    write_fd = open(FIFO2, O_WRONLY); // Open server-to-client pipe for writing

    // Send a welcome message to the client
    char *helloMsg = "Hello, I am the server!, who are you?";
    write(write_fd, helloMsg, strlen(helloMsg) + 1);

    printf("Server sent: %s\n", helloMsg);

    // Read from the client
    read(read_fd, buffer, sizeof(buffer));
    printf("Client sent: %s\n", buffer);

    // send a response to the client
    char *response = "Nice to meet you, have a good day!";
    write(write_fd, response, strlen(response) + 1);
    printf("Server sent: %s\n", response);

    close(read_fd);
    close(write_fd);
    return 0;
}
